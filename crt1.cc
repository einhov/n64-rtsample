#include <string_view>
#include <array>

#include <cerrno>
#undef errno
extern int errno;

void emu_print(const std::string_view &message) {
	for(const char c : message) {
		asm("mtc0 %0, $25" :: "r"(c));
	}
}

extern "C"
void _exit([[maybe_unused]]int status) {
	emu_print("===EXIT CALLED===\n");
	for(;;);
}

extern "C"
void *sbrk(int nbytes) {
	static auto heap_head = reinterpret_cast<char*>(0x80300000);

	if(heap_head + nbytes > reinterpret_cast<char*>(0x80700000)) {
		emu_print("Out of memory\n");
		errno = ENOMEM;
		return nullptr;
	}

	const auto result = heap_head;
	heap_head += nbytes;
	return result;
}

extern "C"
int kill(int, int) {
	errno = EINVAL;
	return -1;
}

extern "C"
int getpid() {
	return 1;
}

extern "C"
int isatty(int) {
	errno = ENOTTY;
	return 0;
}

extern "C"
int close(int) {
	errno = EBADF;
	return -1;
}

extern "C"
ssize_t read(int, void*, size_t) {
	errno = EINVAL;
	return -1;
}

extern "C"
int fstat(int, void*) {
	errno = EBADF;
	return -1;
}

extern "C"
off_t lseek(int, off_t, int) {
	errno = EBADF;
	return -1;
}

extern "C"
ssize_t write(int fd, const void *buf, size_t count) {
	if(fd != 1) {
	   	errno = EINVAL;
		return -1;
	}

	const auto cbuf = static_cast<const char*>(buf);
	for(size_t i = 0; i < count; i++)
		asm("mtc0 %0, $25" :: "r"(cbuf[i]));

	return count;
}

extern "C"
int open(const char*, int, int) {
	errno = EIO;
	return -1;
}

