
// This file has been auto-generated by the codegen tool by running codegen either
// directly or indirectly on:  layer.cpp
//
// Do not change this file manually as any change will be automatically overwritten,
// instead change the struct tagged with "codegen::Dictionary" in the main file from which
// the code in this file was generated.
//
// If a compiler error brought you here, a struct tagged with "codegen::Dictionary"
// was changed without the codegen tool being run again.

namespace codegen {
template <typename T> openspace::documentation::Documentation doc() {
    static_assert(sizeof(T) == 0); // This should never be called
}
template <> openspace::documentation::Documentation doc<openspace::globebrowsing::Layer>() {
    using namespace openspace::documentation;
    TableVerifier* codegen_Parameters = new TableVerifier;
    codegen_Parameters->documentations.push_back({"Identifier",new StringVerifier,Optional::No,"The unique identifier for this layer. May not contain '.' or spaces"});
    codegen_Parameters->documentations.push_back({"Name",new StringVerifier,Optional::Yes,"A human-readable name for the user interface. If this is omitted, the identifier is used instead"});
    codegen_Parameters->documentations.push_back({"Description",new StringVerifier,Optional::Yes,"A human-readable description of the layer to be used in informational texts presented to the user"});
    codegen_Parameters->documentations.push_back({"Color",new DoubleVector3Verifier,Optional::Yes,ColorInfo.description});
    codegen_Parameters->documentations.push_back({"Type",new InListVerifier<StringVerifier>({"DefaultTileLayer", "SingleImageTileLayer", "SizeReferenceTileLayer", "TemporalTileLayer", "TileIndexTileLayer", "ByIndexTileLayer", "ByLevelTileLayer", "SolidColor"}),Optional::Yes,"Specifies the type of layer that is to be added. If this value is not specified, the layer is a DefaultTileLayer"});
    codegen_Parameters->documentations.push_back({"Enabled",new BoolVerifier,Optional::Yes,"Determine whether the layer is enabled or not. If this value is not specified, the layer is disabled"});
    codegen_Parameters->documentations.push_back({"PadTiles",new BoolVerifier,Optional::Yes,"Determines whether the downloaded tiles should have a padding added to the borders"});
    TableVerifier* codegen_Parameters_Settings = new TableVerifier;
    codegen_Parameters_Settings->documentations.push_back({"Opacity",new InRangeVerifier<DoubleVerifier>(0.0, 1.0),Optional::Yes,"The opacity value of the layer"});
    codegen_Parameters_Settings->documentations.push_back({"Gamma",new DoubleVerifier,Optional::Yes,"The gamma value that is applied to each pixel of the layer"});
    codegen_Parameters_Settings->documentations.push_back({"Multiplier",new DoubleVerifier,Optional::Yes,"The multiplicative factor that is applied to each pixel of the layer"});
    codegen_Parameters_Settings->documentations.push_back({"Offset",new DoubleVerifier,Optional::Yes,"An additive offset that is applied to each pixel of the layer"});
    codegen_Parameters->documentations.push_back({"Settings",codegen_Parameters_Settings,Optional::Yes,"Specifies the render settings that should be applied to this layer"});
    TableVerifier* codegen_Parameters_LayerAdjustment = new TableVerifier;
    codegen_Parameters_LayerAdjustment->documentations.push_back({"Type",new InListVerifier<StringVerifier>({"None", "ChromaKey", "TransferFunction"}),Optional::Yes,"Specifies the type of the adjustment that is applied"});
    codegen_Parameters_LayerAdjustment->documentations.push_back({"ChromaKeyColor",new DoubleVector3Verifier,Optional::Yes,"Specifies the chroma key used when selecting 'ChromaKey' for the 'Type'"});
    codegen_Parameters_LayerAdjustment->documentations.push_back({"ChromaKeyTolerance",new DoubleVerifier,Optional::Yes,"Specifies the tolerance to match the color to the chroma key when the 'ChromaKey' type is selected for the 'Type'"});
    codegen_Parameters->documentations.push_back({"Adjustment",codegen_Parameters_LayerAdjustment,Optional::Yes,""});
    codegen_Parameters->documentations.push_back({"BlendMode",new InListVerifier<StringVerifier>({"Normal", "Multiply", "Add", "Subtract", "Color"}),Optional::Yes,"Sets the blend mode of this layer to determine how it interacts with other layers on top of this"});
    codegen_Parameters->documentations.push_back({"Fallback",new ReferencingVerifier("globebrowsing_layer"),Optional::Yes,"If the primary layer creation fails, this layer is used as a fallback"});

    openspace::documentation::Documentation d = {
        "Layer",
        "Layer",
        std::move(codegen_Parameters->documentations)
    };
    delete codegen_Parameters;
    return d;
}
} // namespace codegen


namespace codegen {
namespace internal {
template<typename T> void bakeTo(const ghoul::Dictionary&, std::string_view, T*) { static_assert(sizeof(T) == 0); } // This should never be called
template<typename T> void bakeTo(const ghoul::Dictionary& d, std::string_view key, std::optional<T>* val);
void bakeTo(const ghoul::Dictionary& d, std::string_view key, bool* val) { *val = d.value<bool>(key); }
void bakeTo(const ghoul::Dictionary& d, std::string_view key, double* val) { *val = d.value<double>(key); }
void bakeTo(const ghoul::Dictionary& d, std::string_view key, float* val) { *val = static_cast<float>(d.value<double>(key)); }
void bakeTo(const ghoul::Dictionary& d, std::string_view key, glm::dvec3* val) { *val = d.value<glm::dvec3>(key); }
void bakeTo(const ghoul::Dictionary& d, std::string_view key, glm::vec3* val) { *val = d.value<glm::dvec3>(key); }
void bakeTo(const ghoul::Dictionary&, std::string_view, std::monostate* val) { *val = std::monostate(); }

template<typename T> void bakeTo(const ghoul::Dictionary& d, std::string_view key, std::optional<T>* val) {
    if (d.hasKey(key)) {
        T v;
        bakeTo(d, key, &v);
        *val = v;
    }
    else *val = std::nullopt;
}
void bakeTo(const ghoul::Dictionary& d, std::string_view key, std::string* val) { *val = d.value<std::string>(key); }
template <> void bakeTo<Parameters::Settings>(const ghoul::Dictionary& d, std::string_view key, Parameters::Settings* val) {
    Parameters::Settings& res = *val;
    ghoul::Dictionary dict = d.value<ghoul::Dictionary>(key);
    internal::bakeTo(dict, "Opacity", &res.opacity);
    internal::bakeTo(dict, "Gamma", &res.gamma);
    internal::bakeTo(dict, "Multiplier", &res.multiplier);
    internal::bakeTo(dict, "Offset", &res.offset);
}
template <> void bakeTo<Parameters::LayerAdjustment>(const ghoul::Dictionary& d, std::string_view key, Parameters::LayerAdjustment* val) {
    Parameters::LayerAdjustment& res = *val;
    ghoul::Dictionary dict = d.value<ghoul::Dictionary>(key);
    internal::bakeTo(dict, "Type", &res.type);
    internal::bakeTo(dict, "ChromaKeyColor", &res.chromaKeyColor);
    internal::bakeTo(dict, "ChromaKeyTolerance", &res.chromaKeyTolerance);
}

} // namespace internal

template <typename T> T bake(const ghoul::Dictionary&) { static_assert(sizeof(T) == 0); };
template <> Parameters bake<Parameters>(const ghoul::Dictionary& dict) {
    openspace::documentation::testSpecificationAndThrow(codegen::doc<openspace::globebrowsing::Layer>(), dict, "Layer");
    Parameters res;
    internal::bakeTo(dict, "Identifier", &res.identifier);
    internal::bakeTo(dict, "Name", &res.name);
    internal::bakeTo(dict, "Description", &res.description);
    internal::bakeTo(dict, "Color", &res.color);
    internal::bakeTo(dict, "Type", &res.type);
    internal::bakeTo(dict, "Enabled", &res.enabled);
    internal::bakeTo(dict, "PadTiles", &res.padTiles);
    internal::bakeTo(dict, "Settings", &res.settings);
    internal::bakeTo(dict, "Adjustment", &res.adjustment);
    internal::bakeTo(dict, "BlendMode", &res.blendMode);
    internal::bakeTo(dict, "Fallback", &res.fallback);
    return res;
}
} // namespace codegen
