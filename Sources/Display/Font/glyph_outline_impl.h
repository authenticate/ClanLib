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
**    Mark Page
*/

#pragma once


#include "Display/precomp.h"
#include "glyph_outline.h"
#include "glyph_contour.h"
#include "glyph_outline_impl.h"
#include <vector>
#include "API/Display/Render/primitives_array.h"
#include "API/Display/2D/color.h"
#include "API/Core/Math/ear_clip_triangulator.h"
#include "API/Core/System/databuffer.h"

namespace clan
{

class GraphicContext;
class Canvas;

class GlyphOutline_Impl
{
/// \name Construction
/// \{

public:
	GlyphOutline_Impl();
	virtual ~GlyphOutline_Impl();


/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:

	void add_contour(GlyphContour &contour);

	void triangulate(GlyphPrimitivesArray *out_primitives_array, GlyphPrimitivesArrayOutline *out_primitives_array_outline, GlyphPrimitivesJoinedOutlines *out_joined_outlines);

/// \}
/// \name Implementation
/// \{

private:

	void generate_contour_prim_array(GlyphPrimitivesArrayOutline *out_primitives_array_outline);

	std::vector< GlyphContour > contours;
/// \}
};

}