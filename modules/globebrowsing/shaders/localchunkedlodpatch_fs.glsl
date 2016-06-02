/*****************************************************************************************
 *                                                                                       *
 * OpenSpace                                                                             *
 *                                                                                       *
 * Copyright (c) 2014                                                                    *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "Software"), to deal in the Software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
 * permit persons to whom the Software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the Software.                                              *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
 ****************************************************************************************/

#include <${MODULE_GLOBEBROWSING}/shaders/texturetile.hglsl>
#include <${MODULE_GLOBEBROWSING}/shaders/blending.hglsl>
#include "PowerScaling/powerScaling_fs.hglsl"
#include "fragment.glsl"

#define NUMLAYERS_COLORTEXTURE #{numLayersColor}
#define NUMLAYERS_HEIGHTMAP #{numLayersHeight}

uniform TextureTile colorTiles[NUMLAYERS_COLORTEXTURE];
uniform TextureTile colorTilesParent1[NUMLAYERS_COLORTEXTURE];
uniform TextureTile colorTilesParent2[NUMLAYERS_COLORTEXTURE];

in vec4 fs_position;
in vec2 fs_uv;
in vec3 positionCameraSpace;

uniform float distanceScaleFactor;
uniform int chunkLevel;

Fragment getFragment() {
	Fragment frag;

    // Calculate desired level based on distance
	float distToFrag = length(positionCameraSpace);
    float projectedScaleFactor = distanceScaleFactor / distToFrag;
	float desiredLevel = log2(projectedScaleFactor);

	// x increases with distance
	float x = chunkLevel - desiredLevel;
	float w1 = clamp(1 - x, 0 , 1);
	float w2 =  (clamp(x, 0 , 1) - clamp(x - 1, 0 , 1));
	float w3 = clamp(x - 1, 0 , 1);

	#for j in 1..#{numLayersColor}
	{
		int i = #{j} - 1;

		vec2 samplePos =
			colorTiles[i].uvTransform.uvScale * fs_uv +
			colorTiles[i].uvTransform.uvOffset;
		vec2 samplePosParent1 =
			colorTilesParent1[i].uvTransform.uvScale * fs_uv +
			colorTilesParent1[i].uvTransform.uvOffset;
		vec2 samplePosParent2 =
			colorTilesParent2[i].uvTransform.uvScale * fs_uv +
			colorTilesParent2[i].uvTransform.uvOffset;
		
		vec4 colorSample =
			w1 * texture(colorTiles[i].textureSampler, samplePos) +
			w2 * texture(colorTilesParent1[i].textureSampler, samplePosParent1) +
			w3 * texture(colorTilesParent2[i].textureSampler, samplePosParent2);
		frag.color = blendOver(frag.color, colorSample);
	}
	#endfor

	//vec2 samplePos =
	//	colorTile.uvTransform.uvScale * fs_uv +
	//	colorTile.uvTransform.uvOffset;
	//frag.color = texture(colorTile.textureSampler, samplePos);

	//frag.color.rgb *= 10;

	// Sample position overlay
	//frag.color = frag.color * 0.9 + 0.2*vec4(samplePos, 0, 1);

	// Border overlay
	//frag.color = frag.color + patchBorderOverlay(fs_uv, vec3(0.5, 0.5, 0.5), 0.02);

	frag.depth = fs_position.w;

	return frag;
}

