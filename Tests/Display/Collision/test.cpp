/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Harry Storbacka
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

void print_usage()
{
	// Explain the keys in the terminal
	Console::write_line( "----------: Controls :----------");
	Console::write_line( "Arrows: move the triangle");
	Console::write_line( "Shift+Arrows: move the other outline");
	Console::write_line( "'e':    rotate triangle");
	Console::write_line( "'r':    rotate other figure");
	Console::write_line( "'i':    toggle drawing of sub-circles on triangle");
	Console::write_line( "'o':    toggle drawing of sub-circles on other figure");
	Console::write_line( "'t':    toggle drawing of get_minimum_enclosing_disc on triangle");
	Console::write_line( "'y':    toggle drawing of get_minimum_enclosing_disc on other figure");
	Console::write_line( "'s':    toggle drawing of surfaces");
	Console::write_line( "+/-:    (in/de)crease the radius-multiplier");
	Console::write_line( "'g':    use smallest enclosing disc only");
	Console::write_line( "'c':    replace contours with convex hull");
	Console::write_line( "'h':    show this help again");
	Console::write_line( "1-8:    scale the middle outline");
	Console::write_line( "'x':    save then reload outline");
	Console::write_line( "'d':    toggle drawing of deep point");
};


class App
{
public:
	int start(const std::vector<std::string> &args);

private:
	void on_input_up(const InputEvent &key, const InputState &state);
	void on_window_close();
	void draw_point_normal(GraphicContext &gc, const Pointf &point, const Pointf &normal, const Colorf &color);
private:
	bool quit;
};

class Program
{
public:
	static int main(const std::vector<std::string> &args)
	{
		SetupCore setup_core;
		SetupDisplay setup_display;
		SetupGL setup_gl;

		App app;
		return app.start(args);
	}
};

Application app(&Program::main);

int App::start(const std::vector<std::string> &args)
{
	quit = false;

	ConsoleWindow console("Console", 80, 200);

	print_usage();

	try
	{
		DisplayWindowDescription desc;
		desc.set_title("ClanLib Collision Test");
		desc.set_size(Size(950, 700), true);
		DisplayWindow window(desc);

		Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
		Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

		GraphicContext gc = window.get_gc();

		Font font(gc, "Tahoma", 16);

//////////////////////////////////////////////////////////////////////////
		std::string file1("images/triangle.png");
		//std::string file2("images/triangle.png");
		std::string file2("images/weird.png");
		//std::string file2("images/edge_test2.png");
		//std::string file2("images/inside_test5.png");

		if( args.size() == 3 )
		{
			file1 = args[1];
			file2 = args[2];
		}

		int tri_x_pos = 0;
		int tri_y_pos = 0;
		int other_x_pos = window.get_geometry().get_width()/2;
		int other_y_pos = window.get_geometry().get_height()/2;

		// draw_limit = 0;

		bool draw_min_on_co1 = false;
		bool draw_min_on_co2 = false;
		bool draw_sub_on_co1 = false;
		bool draw_sub_on_co2 = false;
		bool draw_surfaces = false;
		bool draw_deep_point = false;
		float sub_circle_multiplier = 3.5f;

		////////////////////////////////////////////
		// load resources:

		Sprite surface(gc, file1);
		Sprite surface2(gc, file2);

		surface.set_rotation_hotspot(origin_center);
		surface.set_alignment(origin_center);

		surface2.set_rotation_hotspot(origin_center);
		surface2.set_alignment(origin_center);

		////////////////////////////////////////////
		// Collision code

		// load outlines
		VirtualFileSystem vfs(".");
		VirtualDirectory vdir = vfs.get_root_directory();
		CollisionOutline co1(file1);
		CollisionOutline co2(file2, vdir, 128, accuracy_medium, true);

		// Save now before alignment and positions have been applied
		co1.save("collision_1_test_outline_file");
		co2.save("collision_2_test_outline_file");

		// print some info about the outlines:
		unsigned int size = co1.get_contours().size();

		Console::write_line(string_format("outline 1: %1 contour(s)", (int) co1.get_contours().size()));
		Console::write_line(string_format("outline 2: %1 contour(s)", (int) co2.get_contours().size()));

		std::vector<Contour>::const_iterator it;

		Console::write_line("outline 1:");
		int i=1;
		for( it = co1.get_contours().begin(); it!=co1.get_contours().end(); ++it )
		{
			Console::write_line(string_format("\tcontour %1: %2 points", i, (int) (*it).get_points().size()));
			i++;
		}

		Console::write_line("outline 2:");
		i=1;
		for( it = co2.get_contours().begin(); it!=co2.get_contours().end(); ++it )
		{
			Console::write_line("\tcontour %1: %2 points", i, (int) (*it).get_points().size());
			i++;
		}			

		co1.set_alignment(origin_center);
		co1.set_rotation_hotspot(origin_center);
		co1.enable_collision_info(true,true,true,true);
		co1.set_inside_test(true);

		co2.set_alignment(origin_center);
		co2.set_rotation_hotspot(origin_center);
		co2.enable_collision_info(true,true,true,true);
		co2.set_inside_test(true);

		print_usage();

		InputDevice keyboard = window.get_ic().get_keyboard();

		// Loop until the user hits escape:
		while (!quit)
		{
			gc.clear(Colorf::ghostwhite);

			if (keyboard.get_keycode(KEY_SHIFT))
			{
				// Control Other
				if( keyboard.get_keycode(KEY_RIGHT) )
					other_x_pos+=1;
				if( keyboard.get_keycode(KEY_LEFT) )
					other_x_pos-=1;

				if( keyboard.get_keycode(KEY_UP) )
					other_y_pos-=1;
				if( keyboard.get_keycode(KEY_DOWN) )
					other_y_pos+=1;
			}
			else
			{
				// Control Triangle
				if( keyboard.get_keycode(KEY_RIGHT) )
					tri_x_pos+=1;
				if( keyboard.get_keycode(KEY_LEFT) )
					tri_x_pos-=1;

				if( keyboard.get_keycode(KEY_UP) )
					tri_y_pos-=1;
				if( keyboard.get_keycode(KEY_DOWN) )
					tri_y_pos+=1;
			}

			if( keyboard.get_keycode(KEY_E) )
			{
				surface.rotate(Angle(0.1f, cl_degrees));
				co1.rotate(Angle(0.1f, cl_degrees));
			}
			if( keyboard.get_keycode(KEY_R) )
			{
				co2.rotate(Angle(0.1f, cl_degrees));
				surface2.rotate(Angle(0.1f, cl_degrees));
			}
			if( keyboard.get_keycode(KEY_1) )
			{
				co2.set_scale(1.0f, 1.0f);
				surface2.set_scale(1.0f, 1.0f);
				System::sleep(100);
			}
			if( keyboard.get_keycode(KEY_2) )
			{
				co2.set_scale(2.0f, 2.0f);
				surface2.set_scale(2.0f, 2.0f);
				System::sleep(100);
			}
			if( keyboard.get_keycode(KEY_3) )
			{
				co2.set_scale(3.0f, 3.0f);
				surface2.set_scale(3.0f, 3.0f);
				System::sleep(100);
			}
			if( keyboard.get_keycode(KEY_4) )
			{
				co2.set_scale(4.0f, 4.0f);
				surface2.set_scale(4.0f, 4.0f);
				System::sleep(100);
			}
			if( keyboard.get_keycode(KEY_5) )
			{
				co2.set_scale(5.0f, 5.0f);
				surface2.set_scale(5.0f, 5.0f);
				System::sleep(100);
			}
			if( keyboard.get_keycode(KEY_6) )
			{
				co2.set_scale(6.0f, 6.0f);
				surface2.set_scale(6.0f, 6.0f);
				System::sleep(100);
			}
			if( keyboard.get_keycode(KEY_7) )
			{
				co2.set_scale(7.0f, 7.0f);
				surface2.set_scale(7.0f, 7.0f);
				System::sleep(100);
			}
			if( keyboard.get_keycode(KEY_8) )
			{
				co2.set_scale(8.0f, 8.0f);
				surface2.set_scale(8.0f, 8.0f);
				System::sleep(100);
			}
			if( keyboard.get_keycode(KEY_I) )
			{
				draw_sub_on_co1 = !draw_sub_on_co1;
				System::sleep(100);
			}
			if( keyboard.get_keycode(KEY_O) )
			{
				draw_sub_on_co2 = !draw_sub_on_co2;
				System::sleep(100);
			}
			if( keyboard.get_keycode(KEY_T) )
			{
				draw_min_on_co1 = !draw_min_on_co1;
				System::sleep(100);
			}
			if( keyboard.get_keycode(KEY_Y) )
			{
				draw_min_on_co2 = !draw_min_on_co2;
				System::sleep(100);
			}
			if( keyboard.get_keycode(KEY_D) )
			{
				draw_deep_point = !draw_deep_point;
				System::sleep(100);
			}
			if( keyboard.get_keycode(KEY_S) )
			{
				draw_surfaces = !draw_surfaces;
				System::sleep(100);
			}
			if( keyboard.get_keycode(KEY_X) )
			{
				// Load, ensuring recreated
				co1 = CollisionOutline();
				co1.load("collision_1_test_outline_file");
				co2 = CollisionOutline();
				co2.load("collision_2_test_outline_file");
		
				// Reset the options
				co1.set_alignment(origin_center);
				co1.set_rotation_hotspot(origin_center);
				co1.enable_collision_info(true,true,true,true);
				co1.set_inside_test(true);
				co2.set_alignment(origin_center);
				co2.set_rotation_hotspot(origin_center);
				co2.enable_collision_info(true,true,true,true);
				co2.set_inside_test(true);

				System::sleep(100);
			}

			if( keyboard.get_keycode(KEY_SUBTRACT) )
			{
				sub_circle_multiplier -= 0.2f;
				co1.calculate_sub_circles(sub_circle_multiplier);
				co2.calculate_sub_circles(sub_circle_multiplier);
				System::sleep(50);
			}
			if( keyboard.get_keycode(KEY_ADD) )
			{
				sub_circle_multiplier += 0.2f;
				co1.calculate_sub_circles(sub_circle_multiplier);
				co2.calculate_sub_circles(sub_circle_multiplier);
				System::sleep(50);
			}
			if( keyboard.get_keycode(KEY_G) )
			{
				co1.calculate_smallest_enclosing_discs();
				co2.calculate_smallest_enclosing_discs();
				System::sleep(50);
			}
			if( keyboard.get_keycode(KEY_C) )
			{
				co1.calculate_convex_hulls();
				co2.calculate_convex_hulls();
				System::sleep(200);
			}
			if( keyboard.get_keycode(KEY_H) )
			{
				print_usage();
				System::sleep(200);
			}

			// -----------------------------------
			// surfaces
			if(draw_surfaces)
			{
				surface.draw(gc, (float)tri_x_pos, (float)tri_y_pos);
				surface2.draw(gc, (float)other_x_pos, (float)other_y_pos);
			}

			// -----------------------------------
			// co1
			co1.set_translation((float)tri_x_pos, (float)tri_y_pos);
			co1.draw(0.0, 0.0, Colorf::limegreen, gc);
			if(draw_sub_on_co1)
				co1.draw_sub_circles(0.0, 0.0, Colorf::blue, gc);

			// -----------------------------------
			// co2 
			co2.set_translation((float)other_x_pos, (float)other_y_pos);
			co2.draw(0.0, 0.0, Colorf::red, gc );
			if(draw_sub_on_co2)
				co2.draw_sub_circles(0.0, 0.0, Colorf::blue, gc);

			if(draw_min_on_co1)
				Draw::circle(gc, co1.get_minimum_enclosing_disc().position, co1.get_minimum_enclosing_disc().radius, Colorf(0.4f, 0.0f, 0.0f, 0.5f));

			if(draw_min_on_co2)
				Draw::circle(gc, co2.get_minimum_enclosing_disc().position, co2.get_minimum_enclosing_disc().radius, Colorf(0.0f, 0.4f, 0.0f, 0.5f));

			int font_ypos = 20;

			// -----------------------------------
			// collision testing
			if( co2.collide(co1) )
			{
				Draw::fill(gc, gc.get_size(), Colorf(Color(55,40,250,20)));
				const std::vector<CollidingContours> &colpointinfo = co2.get_collision_info();
				for(unsigned int c = 0; c < colpointinfo.size(); c++)
				{
					// Console::write_line(string_format("c1: %1 c2: %2 inside: %3", colpointinfo[c].contour1, colpointinfo[c].contour2, colpointinfo[c].inside));

					for(unsigned int p = 0; p < colpointinfo[c].points.size(); p++)
					{
						const CollisionPoint &collision_point = colpointinfo[c].points[p];
						draw_point_normal(gc, collision_point.point, collision_point.normal, collision_point.is_entry ? Colorf::green : Colorf::red);
	
						// Draw information
						std::string output(string_format("Collision(%1). Point Number (%2). ", c, p));
						output = output + string_format("Point(%1,%2). Normal(%3,%4). ", collision_point.point.x, collision_point.point.y, collision_point.normal.x, collision_point.normal.y);
						if (collision_point.is_entry)
						{
							output = output + string_format("Entry(true). ");
						}
						else
						{
							output = output + string_format("Entry(false). ");
						}

						output = output + string_format("Contour1(%1,%2), Contour2(%3,%4).", collision_point.contour1_line_start, collision_point.contour1_line_end, collision_point.contour2_line_start, collision_point.contour2_line_end);

						font.draw_text(gc, 0, font_ypos, output, Colorf(0.0f, 0.0f, 0.0f, 0.5f));
						font_ypos += 20;
					}
					// Paint the pen-depth and normal from the deepest points
					{
						if (draw_deep_point)
						{
							draw_point_normal(gc, colpointinfo[c].contour1_deep_point, colpointinfo[c].penetration_normal, Colorf::blue);
							draw_point_normal(gc, colpointinfo[c].contour2_deep_point, colpointinfo[c].penetration_normal, Colorf::blue);
						}

						// Draw information

						std::string output(string_format("Collision(%1). ", c));
						if (colpointinfo[c].inside)
						{
							output = output + string_format("Inside(true). ");
						}
						else
						{
							output = output + string_format("Inside(false). ");
						}
						output = output + string_format("PenNormal(%1,%2). ", colpointinfo[c].penetration_normal.x, colpointinfo[c].penetration_normal.y);
						output = output + string_format("PenDepth(%1). ", colpointinfo[c].penetration_depth);
						output = output + string_format("DeepPoint1(%1,%2). ", colpointinfo[c].contour1_deep_point.x, colpointinfo[c].contour1_deep_point.y);
						output = output + string_format("DeepPoint2(%1,%2). ", colpointinfo[c].contour2_deep_point.x, colpointinfo[c].contour2_deep_point.y);
						font.draw_text(gc, 0, font_ypos, output, Colorf(0.0f, 0.0f, 0.0f, 0.5f));
						font_ypos += 20;

					}
				}
			}

			// Update keyboard input and handle system events:
			window.flip(1);
			KeepAlive::process();
		}


//////////////////////////////////////////////////////////////////////////

	}
	catch(Exception& exception)
	{
		Console::write_line("Exception caught:");
		Console::write_line(exception.message);

		// Display the stack trace (if available)
		std::vector<std::string> stacktrace = exception.get_stack_trace();
		int size = stacktrace.size();
		if (size > 0)
		{
			Console::write_line("Stack Trace:");
			for (int cnt=0; cnt < size; cnt++)
			{
				Console::write_line(stacktrace[cnt]);
			}
		}

		console.display_close_message();

		return -1;
	}
	return 0;
}

void App::on_input_up(const InputEvent &key, const InputState &state)
{
	if(key.id == KEY_ESCAPE)
	{
		quit = true;
	}
}

void App::on_window_close()
{
	quit = true;
}

void App::draw_point_normal(GraphicContext &gc, const Pointf &point, const Pointf &normal, const Colorf &color)
{
	Pointf p1 = point;
	Pointf p2 = p1 + Pointf(normal.x * 20.0f, normal.y * 20.0f);
	Draw::line(gc, 
			int(p1.x+0.5f), int(p1.y+0.5f),
			int(p2.x+0.5f), int(p2.y+0.5f),
			color);
}