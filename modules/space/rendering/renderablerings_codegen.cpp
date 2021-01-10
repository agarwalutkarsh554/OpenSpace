
// This file has been auto-generated by the codegen tool by running codegen either
// directly or indirectly on:  renderablerings.cpp
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
template <> openspace::documentation::Documentation doc<openspace::RenderableRings>() {
    using namespace openspace::documentation;
    TableVerifier* codegen_Parameters = new TableVerifier;
    codegen_Parameters->documentations.push_back({"Texture",new StringVerifier,Optional::No,TextureInfo.description});
    codegen_Parameters->documentations.push_back({"Size",new DoubleVerifier,Optional::No,SizeInfo.description});
    codegen_Parameters->documentations.push_back({"Offset",new DoubleVector2Verifier,Optional::Yes,OffsetInfo.description});
    codegen_Parameters->documentations.push_back({"NightFactor",new DoubleVerifier,Optional::Yes,NightFactorInfo.description});
    codegen_Parameters->documentations.push_back({"ColorFilter",new DoubleVerifier,Optional::Yes,ColorFilterInfo.description});

    openspace::documentation::Documentation d = {
        "RenderableRings",
        "RenderableRings",
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
void bakeTo(const ghoul::Dictionary& d, std::string_view key, float* val) { *val = static_cast<float>(d.value<double>(key)); }
void bakeTo(const ghoul::Dictionary& d, std::string_view key, glm::vec2* val) { *val = d.value<glm::dvec2>(key); }

template<typename T> void bakeTo(const ghoul::Dictionary& d, std::string_view key, std::optional<T>* val) {
    if (d.hasKey(key)) {
        T v;
        bakeTo(d, key, &v);
        *val = v;
    }
    else *val = std::nullopt;
}
void bakeTo(const ghoul::Dictionary& d, std::string_view key, std::string* val) { *val = d.value<std::string>(key); }

} // namespace internal

template <typename T> T bake(const ghoul::Dictionary&) { static_assert(sizeof(T) == 0); };
template <> Parameters bake<Parameters>(const ghoul::Dictionary& dict) {
    openspace::documentation::testSpecificationAndThrow(codegen::doc<openspace::RenderableRings>(), dict, "RenderableRings");
    Parameters res;
    internal::bakeTo(dict, "Texture", &res.texture);
    internal::bakeTo(dict, "Size", &res.size);
    internal::bakeTo(dict, "Offset", &res.offset);
    internal::bakeTo(dict, "NightFactor", &res.nightFactor);
    internal::bakeTo(dict, "ColorFilter", &res.colorFilter);
    return res;
}
} // namespace codegen
