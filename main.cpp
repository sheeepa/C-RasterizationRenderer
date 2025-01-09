#include "graphics.h"
#include "iostream"
#include "platform.h"
#include "scene.h"
#include "Renderer.h"
#include "renderbuffer.h"
#include "gameobject.h"
static const char* const WINDOW_TITLE = "RealTimeSoftwareRenderer";
static const int WINDOW_WIDTH = 1280;
static const int WINDOW_HEIGHT = 720;
static const int WINDOW_TEXT_WIDTH = 400;
static const int WINDOW_TEXT_HEIGHT = 400;

struct CallBackControlClass {
	Renderer* Renderer;
	Scene* Scene;
}CallBackList;

int scene_count = 2;
RenderBuffer* frame_buffer = nullptr;

void key_callback(window_t* window, keycode_t key, int pressed) {
	if (pressed)
	{
		CallBackList.Renderer->on_key_input(key, pressed);
	}
}

Object_StaticModel* LoadRenderModel(int scene_index)
{
	switch (scene_index)
	{
	case 0:
		return new Object_StaticModel("../assets/african_head/african_head.obj");
		break;
	case 1:
		return new Object_StaticModel("../assets/diablo3_pose/diablo3_pose.obj");
		break;
	default:
		return new Object_StaticModel("../assets/african_head/african_head.obj");
		break;
	}
};

int main()
{
	//初始化窗口资源，窗口数据，和帧缓冲
	platform_initialize();
	window_t* window;
	Record record = Record();
	callbacks_t callbacks = callbacks_t();
	float prev_time = platform_get_time();
	float print_time = prev_time;
	int num_frames = 0;
	const int text_size = 500;
	char screen_text[text_size];
	int show_num_frames = 0;
	int show_avg_millis = 0;
	float refresh_screen_text_timer = 0;
	const float REFRESH_SCREEN_TEXT_TIME = 0.1;
	snprintf(screen_text, text_size, "fps: - -, avg: - -ms\n");
	window = window_create(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TEXT_WIDTH, WINDOW_TEXT_HEIGHT);
	frame_buffer = new RenderBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

	//设置摄像机参数
	record.window_width = WINDOW_WIDTH;
	record.window_height = WINDOW_HEIGHT;

	//设置窗口参数
	window_set_userdata(window, &record);

	//加载场景
	int scene_index = 0;
	Scene* scene = new Scene("main scene", new Object_StaticModel("../assets/african_head/african_head.obj"));

	Renderer* renderer = new Renderer(frame_buffer, scene);
	CallBackList.Renderer = renderer;
	CallBackList.Scene = scene;

	//注册Windows按键回调
	callbacks.button_callback = button_callback;
	callbacks.scroll_callback = scroll_callback;
	callbacks.key_callback = key_callback;

	//设置窗口输入回调
	input_set_callbacks(window, callbacks);

	while (!window_should_close(window)) {
		float curr_time = platform_get_time();
		float delta_time = curr_time - prev_time;

		// 更新摄像机控制
		update_camera(window, renderer->Camera, &record);

		if (input_key_pressed(window, KEY_W)) {
			scene_index = (scene_index - 1 + scene_count) % scene_count;
			renderer->RenderScene->AddStaticModel(LoadRenderModel(scene_index));
		}
		else if (input_key_pressed(window, KEY_S)) {
			scene_index = (scene_index + 1 + scene_count) % scene_count;
			renderer->RenderScene->AddStaticModel(LoadRenderModel(scene_index));
		}

		// 渲染场景
		renderer->Render();

		// 计算帧率和耗时
		num_frames += 1;
		if (curr_time - print_time >= 1) {
			int sum_millis = (int)((curr_time - print_time) * 1000);
			int avg_millis = sum_millis / num_frames;

			show_num_frames = num_frames;
			show_avg_millis = avg_millis;
			num_frames = 0;
			print_time = curr_time;
		}
		prev_time = curr_time;

		// 把帧缓存绘制到UI窗口
		window_draw_buffer(window, frame_buffer);

		//更新显示文本信息
		refresh_screen_text_timer += delta_time;
		if (refresh_screen_text_timer > REFRESH_SCREEN_TEXT_TIME)
		{
			snprintf(screen_text, text_size, "");

			char line[50] = "";

			snprintf(line, 50, "fps: %3d, avg: %3d ms\n\n", show_num_frames, show_avg_millis);
			strcat(screen_text, line);

			snprintf(line, 50, "scene: %s\n", scene->name);
			strcat(screen_text, line);
			snprintf(line, 50, "press key [W] or [S] to switch scene\n\n");
			strcat(screen_text, line);

			strcat(screen_text, renderer->text);

			window_draw_text(window, screen_text);
			refresh_screen_text_timer -= REFRESH_SCREEN_TEXT_TIME;
		}

		// 重置摄像机输入参数
		record.orbit_delta = Vector2f(0, 0);
		record.pan_delta = Vector2f(0, 0);
		record.dolly_delta = 0;
		record.single_click = 0;
		record.double_click = 0;
	}

	//清除申请的资源
	delete scene;
	delete frame_buffer;
	delete renderer;
	window_destroy(window);
}