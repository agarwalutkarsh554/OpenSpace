/*****************************************************************************************
 *                                                                                       *
 * OpenSpace                                                                             *
 *                                                                                       *
 * Copyright (c) 2014-2021                                                               *
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

#include <modules/atmosphere/rendering/renderableatmosphere.h>

#include <modules/atmosphere/rendering/atmospheredeferredcaster.h>
#include <modules/space/rendering/planetgeometry.h>
#include <openspace/documentation/documentation.h>
#include <openspace/documentation/verifier.h>
#include <openspace/engine/globals.h>
#include <openspace/rendering/deferredcastermanager.h>
#include <openspace/rendering/renderengine.h>
#include <openspace/rendering/renderer.h>
#include <openspace/scene/scenegraphnode.h>
#include <openspace/util/time.h>
#include <openspace/util/spicemanager.h>
#include <ghoul/filesystem/filesystem.h>
#include <ghoul/io/texture/texturereader.h>
#include <ghoul/logging/logmanager.h>
#include <ghoul/misc/assert.h>
#include <ghoul/misc/invariants.h>
#include <ghoul/misc/profiling.h>
#include <ghoul/opengl/programobject.h>
#include <ghoul/opengl/texture.h>
#include <ghoul/opengl/textureunit.h>
#include <glm/gtx/string_cast.hpp>
#include <fstream>
#include <memory>
#include <optional>

#ifdef WIN32
#define _USE_MATH_DEFINES
#endif // WIN32
#include <math.h>

namespace {
    static const char* _loggerCat = "RenderableAtmosphere";

    constexpr const char* KeyShadowGroup  = "ShadowGroup";
    constexpr const char* KeyShadowSource = "Source";
    constexpr const char* KeyShadowCaster = "Caster";

    constexpr const char* KeyPlanetRadius = "PlanetRadius";
    constexpr const char* KeyAverageGroundReflectance = "PlanetAverageGroundReflectance";
    constexpr const char* KeyRayleigh = "Rayleigh";
    constexpr const char* KeyRayleighHeightScale = "H_R";
    constexpr const char* KeyOzone = "Ozone";
    constexpr const char* KeyOzoneHeightScale = "H_O";
    constexpr const char* KeyMie = "Mie";
    constexpr const char* KeyMieHeightScale = "H_M";
    constexpr const char* KeyMiePhaseConstant = "G";
    constexpr const char* KeyImage = "Image";
    constexpr const char* KeyToneMappingOp = "ToneMapping";
    constexpr const char* KeyATMDebug = "Debug";
    constexpr const char* KeyTextureScale = "PreCalculatedTextureScale";
    constexpr const char* KeySaveTextures = "SaveCalculatedTextures";

    constexpr openspace::properties::Property::PropertyInfo AtmosphereHeightInfo {
        "AtmosphereHeight",
        "Atmosphere Height (KM)",
        "The thickness of the atmosphere in km"
    };

    constexpr openspace::properties::Property::PropertyInfo AverageGroundReflectanceInfo {
        "AverageGroundReflectance",
        "Average Ground Reflectance (%)",
        "Average percentage of light reflected by the ground during the pre-calculation "
        "phase"
    };

    constexpr openspace::properties::Property::PropertyInfo GroundRadianceEmittioninfo = {
        "GroundRadianceEmission",
        "Percentage of initial radiance emitted from ground",
        "Multiplier of the ground radiance color during the rendering phase"
    };

    constexpr openspace::properties::Property::PropertyInfo RayleighHeightScaleInfo= {
        "RayleighHeightScale",
        "Rayleigh Scale Height (KM)",
        "It is the vertical distance over which the density and pressure fall by a "
        "constant factor"
    };

    constexpr openspace::properties::Property::PropertyInfo RayleighScatteringCoeffInfo =
    {
        "RayleighScatteringCoeff",
        "Rayleigh Scattering Coeff",
        "Rayleigh sea-level scattering coefficients in meters"
    };

    constexpr openspace::properties::Property::PropertyInfo OzoneLayerInfo = {
        "Ozone",
        "Ozone Layer Enabled",
        "Enables/Disable Ozone Layer during pre-calculation phase"
    };

    constexpr openspace::properties::Property::PropertyInfo OzoneHeightScaleInfo = {
        "OzoneLayerHeightScale",
        "Ozone Scale Height (KM)",
        "It is the vertical distance over which the density and pressure fall by a "
        "constant factor"
    };

    constexpr openspace::properties::Property::PropertyInfo OzoneLayerCoeffInfo = {
        "OzoneLayerCoeff",
        "Ozone Layer Extinction Coeff",
        "Ozone scattering coefficients in meters"
    };

    constexpr openspace::properties::Property::PropertyInfo MieHeightScaleInfo = {
        "MieHeightScale",
        "Mie Scale Height (KM)",
        "It is the vertical distance over which the density and pressure fall by a "
        "constant factor"
    };

    constexpr openspace::properties::Property::PropertyInfo MieScatteringCoeffInfo = {
        "MieScatteringCoeff",
        "Mie Scattering Coeff",
        "Mie sea-level scattering coefficients in meters"
    };

    constexpr openspace::properties::Property::PropertyInfo
    MieScatteringExtinctionPropCoeffInfo =
    {
        "MieScatteringExtinctionPropCoefficient",
        "Mie Scattering/Extinction Proportion Coefficient (%)",
        "Mie Scattering/Extinction Proportion Coefficient (%)"
    };

    constexpr openspace::properties::Property::PropertyInfo MieAsymmetricFactorGInfo = {
        "MieAsymmetricFactorG",
        "Mie Asymmetric Factor G",
        "Averaging of the scattering angle over a high number of scattering events"
    };

    constexpr openspace::properties::Property::PropertyInfo SunIntensityInfo = {
        "SunIntensity",
        "Sun Intensity",
        "Unitless for now"
    };

    constexpr openspace::properties::Property::PropertyInfo
        EnableSunOnCameraPositionInfo =
    {
        "SunFollowingCamera",
        "Enable Sun On Camera Position",
        "When selected the Sun is artificially positioned behind the observer all times"
    };

    constexpr openspace::properties::Property::PropertyInfo EclipseHardShadowsInfo = {
        "EclipseHardShadowsInfo",
        "Enable Hard Shadows for Eclipses",
        "Enable/Disables hard shadows through the atmosphere"
    };
} // namespace

namespace openspace {

struct [[codegen::Dictionary(RenderableAtmosphere)]] Parameters {
    struct ShadowGroup {
        // Individual light sources
        struct SourceElement {
            // The scene graph node name of the source
            std::string name;
            // The radius of the object in meters
            double radius;
        };
        // A list of light sources
        std::vector<SourceElement> sources;

        // Individual shadow casters
        struct CasterElement {
            // The scene graph node name of the source
            std::string name;
            // The radius of the object in meters
            double radius;
        };

        // A list of objects that cast light on this atmosphere
        std::vector<CasterElement> casters;
    };
    // Declares shadow groups, meaning which nodes are considered in shadow calculations
    std::optional<ShadowGroup> shadowGroup;

    // [[codegen::description(AtmosphereHeightInfo)]]
    double atmosphereHeight;

    // The radius of the planet in meters
    double planetRadius;

    double planetAverageGroundReflectance;

    // [[codegen::description(SunIntensityInfo)]]
    std::optional<double> sunIntensity;

    // [[codegen::description(MieScatteringExtinctionPropCoeffInfo)]]
    std::optional<double> mieScatteringExtinctionPropCoefficient;

    // [[codegen::description(GroundRadianceEmittioninfo)]]
    double groundRadianceEmission;

    struct Rayleigh {
        struct Coefficients {
            glm::dvec3 wavelengths;
            glm::dvec3 scattering;
        };
        Coefficients coefficients;
        double heightScale [[codegen::key(H_R)]];
    };
    Rayleigh rayleigh;

    struct Ozone {
        struct Coefficients {
            std::optional<glm::dvec4> extinction;
        };
        std::optional<Coefficients> coefficients;
        std::optional<double> heightScale [[codegen::key(H_O)]];
    };
    std::optional<Ozone> ozone;

    struct Mie {
        struct Coefficients {
            glm::dvec3 scattering;
            glm::dvec3 extinction;
        };
        Coefficients coefficients;
        double heightScale [[codegen::key(H_M)]];
        double phaseConstant [[codegen::key(G), codegen::inrange(-1.0, 1.0)]];
    };
    Mie mie;

    struct ATMDebug {
        std::optional<double> preCalculatedTextureScale [[codegen::inrange(0.0, 1.0)]];
        std::optional<bool> saveCalculatedTextures;
    };
    std::optional<ATMDebug> debug;
};
#include "renderableatmosphere_codegen.cpp"


documentation::Documentation RenderableAtmosphere::Documentation() {
    return codegen::doc<RenderableAtmosphere>();
}

RenderableAtmosphere::RenderableAtmosphere(const ghoul::Dictionary& dictionary)
    : Renderable(dictionary)
    , _atmosphereHeight(AtmosphereHeightInfo, 60.f, 0.1f, 99.0f)
    , _groundAverageReflectance(AverageGroundReflectanceInfo, 0.f, 0.f, 1.f)
    , _groundRadianceEmission(GroundRadianceEmittioninfo, 0.f, 0.f, 1.f)
    , _rayleighHeightScale(RayleighHeightScaleInfo, 0.f, 0.1f, 20.f)
    , _rayleighScatteringCoeff(
        RayleighScatteringCoeffInfo,
        glm::vec3(0.f), glm::vec3(0.00001f), glm::vec3(0.1f)
    )
    , _ozoneEnabled(OzoneLayerInfo, false)
    , _ozoneHeightScale(OzoneHeightScaleInfo, 0.f, 0.1f, 20.f)
    , _ozoneCoeff(
        OzoneLayerCoeffInfo,
        glm::vec3(0.f), glm::vec3(0.00001f), glm::vec3(0.001f)
    )
    , _mieHeightScale(MieHeightScaleInfo, 0.f, 0.1f, 20.f)
    , _mieScatteringCoeff(
        MieScatteringCoeffInfo,
        glm::vec3(0.004f), glm::vec3(0.00001f), glm::vec3(1.f)
    )
    , _mieScatteringExtinctionPropCoefficient(
        MieScatteringExtinctionPropCoeffInfo,
        0.9f, 0.01f, 1.f
    )
    , _miePhaseConstant(MieAsymmetricFactorGInfo, 0.f, -1.f, 1.f)
    , _sunIntensity(SunIntensityInfo, 5.f, 0.1f, 1000.f)
    , _sunFollowingCameraEnabled(EnableSunOnCameraPositionInfo, false)
    , _hardShadowsEnabled(EclipseHardShadowsInfo, false)
 {
    auto updateWithCalculation = [this]() {
        _deferredCasterNeedsUpdate = true;
        _deferredCasterNeedsCalculation = true;
    };
    auto updateWithoutCalculation = [this]() {
        _deferredCasterNeedsUpdate = true;
    };


    const Parameters p = codegen::bake<Parameters>(dictionary);

    _shadowEnabled = p.shadowGroup.has_value();
    if (_shadowEnabled) {
        for (const Parameters::ShadowGroup::SourceElement& s : p.shadowGroup->sources) {
            for (const Parameters::ShadowGroup::CasterElement& c :
                 p.shadowGroup->casters)
            {
                ShadowConfiguration sc;
                sc.source = std::pair(s.name, s.radius);
                sc.caster = std::pair(c.name, c.radius);
                _shadowConfArray.push_back(sc);
            }
        }
    }

    _atmosphereHeight = static_cast<float>(p.atmosphereHeight);
    _atmosphereHeight.onChange(updateWithCalculation);
    addProperty(_atmosphereHeight);

    _planetRadius = static_cast<float>(p.planetRadius);

    _groundAverageReflectance = static_cast<float>(p.planetAverageGroundReflectance);
    _groundAverageReflectance.onChange(updateWithCalculation);
    addProperty(_groundAverageReflectance);

    if (p.sunIntensity.has_value()) {
        _sunIntensity = static_cast<float>(*p.sunIntensity);
    }
    _sunIntensity.onChange(updateWithoutCalculation);
    addProperty(_sunIntensity);

    if (p.mieScatteringExtinctionPropCoefficient.has_value()) {
        _mieScattExtPropCoefProp = static_cast<float>(
            *p.mieScatteringExtinctionPropCoefficient
        );
    }

    _rayleighScatteringCoeff = p.rayleigh.coefficients.scattering;
    _rayleighScatteringCoeff.onChange(updateWithCalculation);
    addProperty(_rayleighScatteringCoeff);

    _rayleighHeightScale = static_cast<float>(p.rayleigh.heightScale);
    _rayleighHeightScale.onChange(updateWithCalculation);
    addProperty(_rayleighHeightScale);

    if (p.ozone.has_value()) {
        if (p.ozone->heightScale.has_value()) {
            _ozoneHeightScale = *p.ozone->heightScale;
            _ozoneEnabled = true;
        }

        if (p.ozone->coefficients.has_value() &&
            p.ozone->coefficients->extinction.has_value())
        {
            _ozoneCoeff = *p.ozone->coefficients->extinction;
        }
    }
    _ozoneEnabled.onChange(updateWithCalculation);
    addProperty(_ozoneEnabled);
    _ozoneHeightScale.onChange(updateWithCalculation);
    addProperty(_ozoneHeightScale);
    _ozoneCoeff.onChange(updateWithCalculation);
    addProperty(_ozoneCoeff);


    _mieHeightScale = static_cast<float>(p.mie.heightScale);
    _mieHeightScale.onChange(updateWithCalculation);
    addProperty(_mieHeightScale);

    _mieScatteringCoeff = p.mie.coefficients.scattering;
    _mieScatteringCoeff.onChange(updateWithCalculation);
    addProperty(_mieScatteringCoeff);

    _mieExtinctionCoeff = p.mie.coefficients.extinction;
    _miePhaseConstant = static_cast<float>(p.mie.phaseConstant);
    _miePhaseConstant.onChange(updateWithCalculation);
    addProperty(_miePhaseConstant);

    _mieScatteringExtinctionPropCoefficient =
        _mieScattExtPropCoefProp != 1.f ? _mieScattExtPropCoefProp :
        _mieScatteringCoeff.value().x / _mieExtinctionCoeff.x;

    _mieScatteringExtinctionPropCoefficient.onChange(updateWithCalculation);
    addProperty(_mieScatteringExtinctionPropCoefficient);
    
    if (p.debug.has_value()) {
        if (p.debug->preCalculatedTextureScale.has_value()) {
            _preCalculatedTexturesScale = static_cast<float>(
                *p.debug->preCalculatedTextureScale
            );
        }
        if (p.debug->saveCalculatedTextures.has_value()) {
            _saveCalculationsToTexture = *p.debug->saveCalculatedTextures;
        }
    }

    _groundRadianceEmission = p.groundRadianceEmission;
    _groundRadianceEmission.onChange(updateWithoutCalculation);
    addProperty(_groundRadianceEmission);

    _sunFollowingCameraEnabled.onChange(updateWithoutCalculation);
    addProperty(_sunFollowingCameraEnabled);

    if (_shadowEnabled) {
        _hardShadowsEnabled.onChange(updateWithoutCalculation);
        addProperty(_hardShadowsEnabled);
    }
}

void RenderableAtmosphere::deinitializeGL() {
    global::deferredcasterManager->detachDeferredcaster(*_deferredcaster);
    _deferredcaster = nullptr;
}

void RenderableAtmosphere::initializeGL() {
    _deferredcaster = std::make_unique<AtmosphereDeferredcaster>();
    _deferredcaster->setAtmosphereRadius(_planetRadius + _atmosphereHeight);
    _deferredcaster->setPlanetRadius(_planetRadius);
    _deferredcaster->setPlanetAverageGroundReflectance(_groundAverageReflectance);
    _deferredcaster->setPlanetGroundRadianceEmittion(_groundRadianceEmission);
    _deferredcaster->setRayleighHeightScale(_rayleighHeightScale);
    _deferredcaster->enableOzone(_ozoneEnabled);
    _deferredcaster->setOzoneHeightScale(_ozoneHeightScale);
    _deferredcaster->setMieHeightScale(_mieHeightScale);
    _deferredcaster->setMiePhaseConstant(_miePhaseConstant);
    _deferredcaster->setSunRadianceIntensity(_sunIntensity);
    _deferredcaster->setRayleighScatteringCoefficients(_rayleighScatteringCoeff);
    _deferredcaster->setOzoneExtinctionCoefficients(_ozoneCoeff);
    _deferredcaster->setMieScatteringCoefficients(_mieScatteringCoeff);
    _deferredcaster->setMieExtinctionCoefficients(_mieExtinctionCoeff);
    // TODO: Fix the ellipsoid nature of the renderable globe (JCC)
    //_deferredcaster->setEllipsoidRadii(_ellipsoid.radii());
    _deferredcaster->enableSunFollowing(_sunFollowingCameraEnabled);

    _deferredcaster->setPrecalculationTextureScale(_preCalculatedTexturesScale);
    if (_saveCalculationsToTexture)
        _deferredcaster->enablePrecalculationTexturesSaving();

    if (_shadowEnabled) {
        _deferredcaster->setShadowConfigArray(_shadowConfArray);
        // We no longer need it
        _shadowConfArray.clear();
        _deferredcaster->setHardShadows(_hardShadowsEnabled);
    }

    _deferredcaster->initialize();

    global::deferredcasterManager->attachDeferredcaster(*_deferredcaster);
}

bool RenderableAtmosphere::isReady() const {
    return true;
}

glm::dmat4 RenderableAtmosphere::computeModelTransformMatrix(
                                                       const TransformData& transformData)
{
    // scale the planet to appropriate size since the planet is a unit sphere
    return glm::translate(glm::dmat4(1.0), transformData.translation) * // Translation
        glm::dmat4(transformData.rotation) *  // Spice rotation
        glm::scale(glm::dmat4(1.0), glm::dvec3(transformData.scale));
}

void RenderableAtmosphere::render(const RenderData& data, RendererTasks& renderTask) {
    ZoneScoped

    DeferredcasterTask task{ _deferredcaster.get(), data };
    renderTask.deferredcasterTasks.push_back(task);
}

void RenderableAtmosphere::update(const UpdateData& data) {
    if (_deferredCasterNeedsUpdate) {
        updateAtmosphereParameters();
        _deferredCasterNeedsUpdate = false;
    }
    if (_deferredCasterNeedsCalculation) {
        _deferredcaster->preCalculateAtmosphereParam();
        _deferredCasterNeedsCalculation = false;
    }

    _deferredcaster->setTime(data.time.j2000Seconds());
    glm::dmat4 modelTransform = computeModelTransformMatrix(data.modelTransform);
    _deferredcaster->setModelTransform(modelTransform);
    _deferredcaster->update(data);
}

void RenderableAtmosphere::updateAtmosphereParameters() {
    _mieExtinctionCoeff =
        _mieScatteringCoeff.value() / _mieScatteringExtinctionPropCoefficient.value();

    _deferredcaster->setAtmosphereRadius(_planetRadius + _atmosphereHeight);
    _deferredcaster->setPlanetRadius(_planetRadius);
    _deferredcaster->setPlanetAverageGroundReflectance(_groundAverageReflectance);
    _deferredcaster->setPlanetGroundRadianceEmittion(_groundRadianceEmission);
    _deferredcaster->setRayleighHeightScale(_rayleighHeightScale);
    _deferredcaster->enableOzone(_ozoneEnabled);
    _deferredcaster->setOzoneHeightScale(_ozoneHeightScale);
    _deferredcaster->setMieHeightScale(_mieHeightScale);
    _deferredcaster->setMiePhaseConstant(_miePhaseConstant);
    _deferredcaster->setSunRadianceIntensity(_sunIntensity);
    _deferredcaster->setRayleighScatteringCoefficients(_rayleighScatteringCoeff);
    _deferredcaster->setOzoneExtinctionCoefficients(_ozoneCoeff);
    _deferredcaster->setMieScatteringCoefficients(_mieScatteringCoeff);
    _deferredcaster->setMieExtinctionCoefficients(_mieExtinctionCoeff);
    _deferredcaster->enableSunFollowing(_sunFollowingCameraEnabled);
    //_deferredcaster->setEllipsoidRadii(_ellipsoid.radii());

    if (_shadowEnabled) {
        _deferredcaster->setHardShadows(_hardShadowsEnabled);
    }
}

}  // namespace openspace
