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

#ifndef __OPENSPACE_MODULE_ATMOSPHERE___ATMOSPHEREDEFERREDCASTER___H__
#define __OPENSPACE_MODULE_ATMOSPHERE___ATMOSPHEREDEFERREDCASTER___H__

#include <openspace/rendering/deferredcaster.h>

#include <modules/atmosphere/rendering/renderableatmosphere.h>
#include <ghoul/glm.h>
#include <ghoul/opengl/textureunit.h>
#include <ghoul/opengl/uniformcache.h>

#include <string>
#include <vector>

namespace ghoul::opengl {
    class Texture;
    class ProgramObject;
} // namespace ghoul::opengl

namespace openspace {

struct RenderData;
struct DeferredcastData;
struct ShadowConfiguration;

class AtmosphereDeferredcaster : public Deferredcaster {
public:
    virtual ~AtmosphereDeferredcaster() = default;

    void initialize();
    void deinitialize();
    void preRaycast(const RenderData& renderData, const DeferredcastData& deferredData,
                    ghoul::opengl::ProgramObject& program) override;
    void postRaycast(const RenderData& renderData, const DeferredcastData& deferredData,
                     ghoul::opengl::ProgramObject& program) override;

    std::string deferredcastPath() const override;
    std::string deferredcastVSPath() const override;
    std::string deferredcastFSPath() const override;
    std::string helperPath() const override;

    void initializeCachedVariables(ghoul::opengl::ProgramObject&) override;

    void update(const UpdateData&) override;

    void preCalculateAtmosphereParam();

    void setModelTransform(const glm::dmat4 &transform);
    void setTime(double time);
    void setAtmosphereRadius(float atmRadius);
    void setPlanetRadius(float planetRadius);
    void setPlanetAverageGroundReflectance(float averageGReflectance);
    void setPlanetGroundRadianceEmittion(float groundRadianceEmittion);
    void setRayleighHeightScale(float rayleighHeightScale);
    void enableOzone(bool enable);
    void setOzoneHeightScale(float ozoneHeightScale);
    void setMieHeightScale(float mieHeightScale);
    void setMiePhaseConstant(float miePhaseConstant);
    void setSunRadianceIntensity(float sunRadiance);
    void setRayleighScatteringCoefficients(glm::vec3 rayScattCoeff);
    void setOzoneExtinctionCoefficients(glm::vec3 ozoneExtCoeff);
    void setMieScatteringCoefficients(glm::vec3 mieScattCoeff);
    void setMieExtinctionCoefficients(glm::vec3 mieExtCoeff);
    void setEllipsoidRadii(glm::dvec3 radii);
    void setShadowConfigArray(std::vector<ShadowConfiguration> shadowConfigArray);
    void setHardShadows(bool enabled);
    void enableSunFollowing(bool enable);

    void setPrecalculationTextureScale(float preCalculatedTexturesScale);
    void enablePrecalculationTexturesSaving();

private:
    void loadComputationPrograms();
    void unloadComputationPrograms();
    void createComputationTextures();
    void deleteComputationTextures();
    void deleteUnusedComputationTextures();
    void executeCalculations(GLuint quadCalcVAO, GLenum drawBuffers[1],
        GLsizei vertexSize);
    void step3DTexture(std::unique_ptr<ghoul::opengl::ProgramObject>& shaderProg,
        int layer, bool doCalculation = true);
    void checkFrameBufferState(const std::string& codePosition) const;
    void loadAtmosphereDataIntoShaderProgram(
        std::unique_ptr<ghoul::opengl::ProgramObject> & shaderProg
    );
    void renderQuadForCalc(GLuint vao, GLsizei numberOfVertices);
    void saveTextureToPPMFile(GLenum color_buffer_attachment, const std::string& fileName,
        int width, int height) const;
    bool isAtmosphereInFrustum(const glm::dmat4& MVMatrix, const glm::dvec3& position,
        double radius) const;

    // Number of planet radii to use as distance threshold for culling
    const double DISTANCE_CULLING_RADII = 5000;

    std::unique_ptr<ghoul::opengl::ProgramObject> _transmittanceProgramObject;
    std::unique_ptr<ghoul::opengl::ProgramObject> _irradianceProgramObject;
    std::unique_ptr<ghoul::opengl::ProgramObject> _irradianceSupTermsProgramObject;
    std::unique_ptr<ghoul::opengl::ProgramObject> _irradianceFinalProgramObject;
    std::unique_ptr<ghoul::opengl::ProgramObject> _inScatteringProgramObject;
    std::unique_ptr<ghoul::opengl::ProgramObject> _inScatteringSupTermsProgramObject;
    std::unique_ptr<ghoul::opengl::ProgramObject> _deltaEProgramObject;
    std::unique_ptr<ghoul::opengl::ProgramObject> _deltaSProgramObject;
    std::unique_ptr<ghoul::opengl::ProgramObject> _deltaSSupTermsProgramObject;
    std::unique_ptr<ghoul::opengl::ProgramObject> _deltaJProgramObject;
    std::unique_ptr<ghoul::opengl::ProgramObject> _atmosphereProgramObject;
    std::unique_ptr<ghoul::opengl::ProgramObject> _deferredAtmosphereProgramObject;

    UniformCache(cullAtmosphere, Rg, Rt,
        groundRadianceEmittion, HR, betaRayleigh, HM,
        betaMieExtinction, mieG, sunRadiance, ozoneLayerEnabled,
        HO, betaOzoneExtinction, SAMPLES_R,
        SAMPLES_MU, SAMPLES_MU_S, SAMPLES_NU) _uniformCache;
    UniformCache(dInverseModelTransformMatrix, dModelTransformMatrix,
        dSgctProjectionToModelTransformMatrix,
        dSGCTViewToWorldMatrix, dCamPosObj, sunDirectionObj,
        hardShadows, transmittanceTexture, irradianceTexture,
        inscatterTexture) _uniformCache2;

    GLuint _transmittanceTableTexture = 0;
    GLuint _irradianceTableTexture = 0;
    GLuint _inScatteringTableTexture = 0;
    GLuint _deltaETableTexture = 0;
    GLuint _deltaSRayleighTableTexture = 0;
    GLuint _deltaSMieTableTexture = 0;
    GLuint _deltaJTableTexture = 0;
    GLuint _atmosphereTexture = 0;

    ghoul::opengl::TextureUnit _transmittanceTableTextureUnit;
    ghoul::opengl::TextureUnit _irradianceTableTextureUnit;
    ghoul::opengl::TextureUnit _inScatteringTableTextureUnit;

    // Atmosphere Data
    bool _atmosphereCalculated = false;
    bool _ozoneEnabled = false;
    bool _sunFollowingCameraEnabled = false;
    float _atmosphereRadius = 0.f;
    float _atmospherePlanetRadius = 0.f;
    float _planetAverageGroundReflectance = 0.f;
    float _planetGroundRadianceEmittion = 0.f;
    float _rayleighHeightScale = 0.f;
    float _ozoneHeightScale = 0.f;
    float _mieHeightScale = 0.f;
    float _miePhaseConstant = 0.f;
    float _sunRadianceIntensity = 5.f;

    glm::vec3 _rayleighScatteringCoeff = glm::vec3(0.f);
    glm::vec3 _ozoneExtinctionCoeff = glm::vec3(0.f);
    glm::vec3 _mieScatteringCoeff = glm::vec3(0.f);
    glm::vec3 _mieExtinctionCoeff = glm::vec3(0.f);
    glm::dvec3 _ellipsoidRadii = glm::vec3(0.f);

    // Atmosphere Textures Dimmensions
    int _transmittance_table_width = 256;
    int _transmittance_table_height = 64;
    int _irradiance_table_width = 64;
    int _irradiance_table_height = 16;
    int _delta_e_table_width = 64;
    int _delta_e_table_height = 16;
    int _r_samples = 32;
    int _mu_samples = 128;
    int _mu_s_samples = 32;
    int _nu_samples = 8;

    glm::dmat4 _modelTransform;
    double _time = 0.0;

    // Eclipse Shadows
    std::vector<ShadowConfiguration> _shadowConfArray;
    bool _hardShadowsEnabled = false;

    // Atmosphere Debugging
    float _calculationTextureScale = 1.f;
    bool _saveCalculationTextures = false;

    std::vector<ShadowRenderingStruct> _shadowDataArrayCache;
    // Assuming < 1000 shadow casters, the longest uniform name that we are getting is
    // shadowDataArray[999].casterPositionVec
    // which needs to fit into the uniform buffer
    char _uniformNameBuffer[40];
};

} // openspace

#endif // __OPENSPACE_MODULE_ATMOSPHERE___ATMOSPHEREDEFERREDCASTER___H__
