#include <fmt/format.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/rotate_vector.hpp>

struct video_interface {
	uint32_t STATUS;
	uint32_t ORIGIN;
	uint32_t H_WIDTH;
	uint32_t V_INTR;
	uint32_t V_CURRENT_LINE;
	uint32_t V_TIMING;
	uint32_t V_SYNC;
	uint32_t H_SYNC;
	uint32_t H_SYNC_LEAP;
	uint32_t H_VIDEO;
	uint32_t V_VIDEO;
	uint32_t V_BURST;
	uint32_t X_SCALE;
	uint32_t Y_SCALE;
};

extern volatile video_interface VI;

static constexpr size_t width { 320 };
static constexpr size_t height { 240 };

struct pixel {
	uint16_t r : 5;
	uint16_t g : 5;
	uint16_t b : 5;
	uint16_t : 0;
};
extern pixel framebuffer[width * height];

static void sphere() {
	constexpr glm::vec2 dims { width, height };
	constexpr float ratio = dims.x / dims.y;
	constexpr float tan_vfov = 1.0f;
	constexpr glm::vec3 view { tan_vfov * ratio, tan_vfov, -1.0f };

	constexpr struct { glm::vec3 pos; float size; } sphere {
		{ 0.0f, 0.0f, -1.8f }, { 1.0f }
	};
	constexpr glm::vec3 light { -3.0f, 2.0f, 1.0f };

	static float rotation = 0.0f;
	for(size_t y = 0; y < height; ++y) {
		for(size_t x = 0; x < width; ++x) {
			const glm::vec2 coords { x, y };
			const glm::vec3 ncoords {
				glm::vec2 { coords.x, dims.y - coords.y - 1.0f } / dims * 2.0f - 1.0f,
				1.0f
			};
			const glm::vec3 ray { normalize(ncoords * view) };

			auto &colour = framebuffer[y * width + x];
			colour = { 0, 0, 0 };

			const float t_ca = glm::dot(sphere.pos, ray);
			if(t_ca < 0) continue;
			const float d2 = glm::dot(sphere.pos, sphere.pos) - t_ca * t_ca;
			if(d2 > std::pow(sphere.size, 2.0f)) continue;
			const float t_hc = std::sqrt(sphere.size * sphere.size - d2);
			const float t0 = t_ca - t_hc;

			const glm::vec3 p { ray * t0 };
			const glm::vec3 n { glm::normalize(p - sphere.pos) };
			const glm::vec3 nr { glm::rotate(
				n, glm::radians(rotation), { 0.0f, 1.0f, 0.0f }
			)};
			const glm::vec2 uv {
				0.5f + std::atan2(nr.z, nr.x) / (2.0f * glm::pi<float>()),
				0.5f - std::asin(nr.y) / glm::pi<float>()
			};

			const glm::ivec2 iuv { uv.x * 220, uv.y * 120 };
			const float l = std::max(0.0f, glm::dot(glm::normalize(light - sphere.pos), n));
			const float m = ((iuv.x ^ iuv.y) & 31);
			const unsigned char i = m * l;

			colour = { i, i, i };
		}
	}
	rotation += 5.0f;
}

extern "C" int main() {
	fmt::print("Game title: {}\n", reinterpret_cast<const char *>(0xb0000020));

	VI.STATUS = 0x3246;
	VI.ORIGIN = reinterpret_cast<uint32_t>(&framebuffer[0]);
	VI.H_WIDTH = width;
	VI.V_INTR = 0x2;
	VI.V_CURRENT_LINE = 0;
	VI.V_TIMING = 0x3e52239;
	VI.V_SYNC = 0x20d;
	VI.H_SYNC = 0xc15;
	VI.H_SYNC_LEAP = 0xc150c15UL;
	VI.H_VIDEO = 0x6c02ec;
	VI.V_VIDEO = 0x2501ff;
	VI.V_BURST = 0xe0204;
	VI.X_SCALE = 0x200;
	VI.Y_SCALE = 0x400;

	for(;;) {
		sphere();
	}

	exit(EXIT_SUCCESS);
}
