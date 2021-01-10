
// This file has been auto-generated by the codegen tool by running codegen either
// directly or indirectly on:  constantrotation.cpp
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
template <> openspace::documentation::Documentation doc<openspace::ConstantRotation>() {
    using namespace openspace::documentation;
    TableVerifier* codegen_Parameters = new TableVerifier;
    codegen_Parameters->documentations.push_back({"RotationAxis",new DoubleVector3Verifier,Optional::Yes,RotationInfo.description});
    codegen_Parameters->documentations.push_back({"RotationRate",new DoubleVerifier,Optional::Yes,RotationRateInfo.description});

    openspace::documentation::Documentation d = {
        "ConstantRotation",
        "ConstantRotation",
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
void bakeTo(const ghoul::Dictionary& d, std::string_view key, glm::dvec3* val) { *val = d.value<glm::dvec3>(key); }

template<typename T> void bakeTo(const ghoul::Dictionary& d, std::string_view key, std::optional<T>* val) {
    if (d.hasKey(key)) {
        T v;
        bakeTo(d, key, &v);
        *val = v;
    }
    else *val = std::nullopt;
}

} // namespace internal

template <typename T> T bake(const ghoul::Dictionary&) { static_assert(sizeof(T) == 0); };
template <> Parameters bake<Parameters>(const ghoul::Dictionary& dict) {
    openspace::documentation::testSpecificationAndThrow(codegen::doc<openspace::ConstantRotation>(), dict, "ConstantRotation");
    Parameters res;
    internal::bakeTo(dict, "RotationAxis", &res.rotationAxis);
    internal::bakeTo(dict, "RotationRate", &res.rotationRate);
    return res;
}
} // namespace codegen
