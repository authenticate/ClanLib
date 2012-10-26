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
**    Magnus Norddahl
*/

#pragma once

#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/Font/font.h"
#include "API/Core/Math/mat4.h"
#include "API/Core/Signals/slot.h"
#include "graphic_screen.h"

namespace clan
{

class GraphicContext_Impl : GraphicContext_State
{
public:
	GraphicContext_Impl(GraphicContextProvider *provider);
	GraphicContext_Impl(const GraphicContext_Impl *from_gc, bool clone);
	~GraphicContext_Impl();

	void set_active();

	void set_scissor(const Rect &rect);
	void reset_scissor();

	Size get_size();

	void set_frame_buffer(const FrameBuffer &write_buffer, const FrameBuffer &read_buffer);
	void set_viewport(int index, const Rectf &viewport);
	void set_depth_range(int viewport, float n, float f);

	void set_texture(int unit_index, const Texture &texture);
	void set_textures(std::vector<Texture> &textures);

	void set_image_texture(int unit_index, const Texture &texture);
	void set_image_textures(std::vector<Texture> &textures);

	void set_uniform_buffer(int index, const UniformBuffer &buffer);
	void set_storage_buffer(int index, const StorageBuffer &buffer);

	void set_program_object(StandardProgram standard_program);
	void set_program_object(const ProgramObject &program);
	void reset_program_object();

	void set_rasterizer_state(const RasterizerState &state);
	void set_blend_state(const BlendState &state, const Vec4f &blend_color, unsigned int sample_mask);
	void set_depth_stencil_state(const DepthStencilState &state, int stencil_ref);

	void set_point_size(float);
	void set_point_fade_treshold_size(float);
	void set_line_width(float);
	void enable_vertex_program_point_size(bool enable);
	void set_point_sprite_origin(PointSpriteOrigin origin);

	void set_draw_buffer(DrawBuffer buffer);
	void enable_logic_op(bool enabled);
	void set_logic_op(LogicOp op);

	void set_antialiased(bool value);
	void set_point_offset(bool value);
	void set_line_offset(bool value);
	void set_polygon_offset(bool value);
	void set_offset_factor(float value);
	void set_offset_units(float value);
private:
	void on_window_resized(const Size &size);

	std::shared_ptr<GraphicScreen> graphic_screen;

	Size display_window_size;
	Slot slot_window_resized;

	RasterizerState default_rasterizer_state;
	BlendState default_blend_state;
	DepthStencilState default_depth_stencil_state;

	friend class GraphicScreen;
	friend class GraphicContext;
};

}