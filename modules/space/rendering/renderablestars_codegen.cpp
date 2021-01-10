
// This file has been auto-generated by the codegen tool by running codegen either
// directly or indirectly on:  renderablestars.cpp
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
    return openspace::documentation::Documentation();
}
template <> openspace::documentation::Documentation doc<openspace::RenderableStars>() {
    using namespace openspace::documentation;
    TableVerifier* codegen_Parameters = new TableVerifier;
    codegen_Parameters->documentations.push_back({"File",new StringVerifier,Optional::No,"The path to the SPECK file containing information about the stars being rendered"});
    codegen_Parameters->documentations.push_back({"ColorMap",new StringVerifier,Optional::No,ColorTextureInfo.description});
    codegen_Parameters->documentations.push_back({"ColorOption",new InListVerifier<StringVerifier>({"Color", "Velocity", "Speed", "Other Data", "Fixed Color"}),Optional::Yes,ColorOptionInfo.description});
    codegen_Parameters->documentations.push_back({"OtherData",new StringVerifier,Optional::Yes,OtherDataOptionInfo.description});
    codegen_Parameters->documentations.push_back({"OtherDataColorMap",new StringVerifier,Optional::Yes,OtherDataColorMapInfo.description});
    codegen_Parameters->documentations.push_back({"FilterOutOfRange",new BoolVerifier,Optional::Yes,FilterOutOfRangeInfo.description});
    codegen_Parameters->documentations.push_back({"StaticFilter",new DoubleVerifier,Optional::Yes,"This value specifies a value that is always filtered out of the value ranges on loading. This can be used to trim the dataset's automatic value range"});
    codegen_Parameters->documentations.push_back({"StaticFilterReplacement",new DoubleVerifier,Optional::Yes,"This is the value that is used to replace statically filtered values. Setting this value only makes sense if 'StaticFilter' is 'true', as well"});
    codegen_Parameters->documentations.push_back({"MagnitudeExponent",new DoubleVerifier,Optional::Yes,MagnitudeExponentInfo.description});
    codegen_Parameters->documentations.push_back({"EnableTestGrid",new BoolVerifier,Optional::Yes,EnableTestGridInfo.description});
    codegen_Parameters->documentations.push_back({"RenderMethod",new StringVerifier,Optional::No,RenderMethodOptionInfo.description});
    codegen_Parameters->documentations.push_back({"Texture",new StringVerifier,Optional::No,PsfTextureInfo.description});
    codegen_Parameters->documentations.push_back({"SizeComposition",new StringVerifier,Optional::Yes,SizeCompositionOptionInfo.description});
    codegen_Parameters->documentations.push_back({"FadeInDistances",new DoubleVector2Verifier,Optional::Yes,FadeInDistancesInfo.description});
    codegen_Parameters->documentations.push_back({"DistableFadeIn",new BoolVerifier,Optional::Yes,DisableFadeInInfo.description});

    openspace::documentation::Documentation d = {
        "RenderableStars",
        "RenderableStars",
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
void bakeTo(const ghoul::Dictionary& d, std::string_view key, float* val) { *val = static_cast<float>(d.value<double>(key)); }
void bakeTo(const ghoul::Dictionary& d, std::string_view key, glm::dvec2* val) { *val = d.value<glm::dvec2>(key); }
void bakeTo(const ghoul::Dictionary& d, std::string_view key, std::string* val) { *val = d.value<std::string>(key); }

template<typename T> void bakeTo(const ghoul::Dictionary& d, std::string_view key, std::optional<T>* val) {
    if (d.hasKey(key)) {
        T v;
        bakeTo(d, key, &v);
        *val = v;
    }
    else *val = std::nullopt;
}

} // namespace internal

template <typename T> T bake(const ghoul::Dictionary&) { static_assert(sizeof(T) == 0); }
template <> Parameters bake<Parameters>(const ghoul::Dictionary& dict) {
    openspace::documentation::testSpecificationAndThrow(codegen::doc<openspace::RenderableStars>(), dict, "RenderableStars");
    Parameters res;
    internal::bakeTo(dict, "File", &res.file);
    internal::bakeTo(dict, "ColorMap", &res.colorMap);
    internal::bakeTo(dict, "ColorOption", &res.colorOption);
    internal::bakeTo(dict, "OtherData", &res.otherData);
    internal::bakeTo(dict, "OtherDataColorMap", &res.otherDataColorMap);
    internal::bakeTo(dict, "FilterOutOfRange", &res.filterOutOfRange);
    internal::bakeTo(dict, "StaticFilter", &res.staticFilter);
    internal::bakeTo(dict, "StaticFilterReplacement", &res.staticFilterReplacement);
    internal::bakeTo(dict, "MagnitudeExponent", &res.magnitudeExponent);
    internal::bakeTo(dict, "EnableTestGrid", &res.enableTestGrid);
    internal::bakeTo(dict, "RenderMethod", &res.renderMethod);
    internal::bakeTo(dict, "Texture", &res.texture);
    internal::bakeTo(dict, "SizeComposition", &res.sizeComposition);
    internal::bakeTo(dict, "FadeInDistances", &res.fadeInDistances);
    internal::bakeTo(dict, "DistableFadeIn", &res.distableFadeIn);
    return res;
}
} // namespace codegen
