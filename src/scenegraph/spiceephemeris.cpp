/*****************************************************************************************
 *                                                                                       *
 * OpenSpace                                                                             *
 *                                                                                       *
 * Copyright (c) 2014-2015                                                               *
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

#include <openspace/scenegraph/spiceephemeris.h>

#include <openspace/util/constants.h>
#include <openspace/util/spicemanager.h>
#include <openspace/util/time.h>

namespace {
    const std::string _loggerCat = "SpiceEphemeris";
}

namespace openspace {
    
using namespace constants::spiceephemeris;
    
SpiceEphemeris::SpiceEphemeris(const ghoul::Dictionary& dictionary)
    : _targetName("")
    , _originName("")
    , _position()
	, _kernelsLoadedSuccessfully(true)
{
    const bool hasBody = dictionary.getValue(keyBody, _targetName);
    if (!hasBody)
        LERROR("SpiceEphemeris does not contain the key '" << keyBody << "'");

    const bool hasObserver = dictionary.getValue(keyOrigin, _originName);
    if (!hasObserver)
        LERROR("SpiceEphemeris does not contain the key '" << keyOrigin << "'");

	ghoul::Dictionary kernels;
	dictionary.getValue(keyKernels, kernels);
	for (size_t i = 1; i <= kernels.size(); ++i) {
		std::string kernel;
		bool success = kernels.getValue(std::to_string(i), kernel);
		if (!success)
			LERROR("'" << keyKernels << "' has to be an array-style table");

		SpiceManager::KernelIdentifier id = SpiceManager::ref().loadKernel(kernel);
		_kernelsLoadedSuccessfully &= (id != SpiceManager::KernelFailed);
	}
}
    
const psc& SpiceEphemeris::position() const {
    return _position;
}

void SpiceEphemeris::update(const UpdateData& data) {
	if (!_kernelsLoadedSuccessfully)
		return;

	glm::dvec3 position(0,0,0);
	double lightTime = 0.0;
	SpiceManager::ref().getTargetPosition(_targetName, _originName, 
		"GALACTIC", "NONE", data.time, position, lightTime);

	if (_targetName == "NEW HORIZONS"){
		// In order to properly draw the viewfrustrum, the craft might have to be
		// positioned using the X-variations of aberration methods (ongoing investigation).
		SpiceManager::ref().getTargetPosition(_targetName, _originName,
			"GALACTIC", "NONE", data.time, position, lightTime);
	}
	
	_position = psc::CreatePowerScaledCoordinate(position.x, position.y, position.z);
	_position[3] += 3;
}

} // namespace openspace