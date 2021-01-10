
// This file has been auto-generated by the codegen tool by running codegen either
// directly or indirectly on:  keplertranslation.cpp
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
template <> openspace::documentation::Documentation doc<openspace::KeplerTranslation>() {
    using namespace openspace::documentation;
    TableVerifier* codegen_Parameters = new TableVerifier;
    codegen_Parameters->documentations.push_back({"Eccentricity",new InRangeVerifier<DoubleVerifier>(0.0, 1.0),Optional::No,EccentricityInfo.description});
    codegen_Parameters->documentations.push_back({"SemiMajorAxis",new DoubleVerifier,Optional::No,SemiMajorAxisInfo.description});
    codegen_Parameters->documentations.push_back({"Inclination",new InRangeVerifier<DoubleVerifier>(0.0, 360.0),Optional::No,InclinationInfo.description});
    codegen_Parameters->documentations.push_back({"AscendingNode",new InRangeVerifier<DoubleVerifier>(0.0, 360.0),Optional::No,AscendingNodeInfo.description});
    codegen_Parameters->documentations.push_back({"ArgumentOfPeriapsis",new InRangeVerifier<DoubleVerifier>(0.0, 360.0),Optional::No,ArgumentOfPeriapsisInfo.description});
    codegen_Parameters->documentations.push_back({"MeanAnomaly",new InRangeVerifier<DoubleVerifier>(0.0, 360.0),Optional::No,MeanAnomalyAtEpochInfo.description});
    codegen_Parameters->documentations.push_back({"Epoch",new StringVerifier,Optional::No,EpochInfo.description});
    codegen_Parameters->documentations.push_back({"Period",new GreaterVerifier<DoubleVerifier>(0.0),Optional::No,PeriodInfo.description});

    openspace::documentation::Documentation d = {
        "KeplerTranslation",
        "KeplerTranslation",
        std::move(codegen_Parameters->documentations)
    };
    delete codegen_Parameters;
    return d;
}
} // namespace codegen


namespace codegen {
namespace internal {
template<typename T> void bakeTo(const ghoul::Dictionary&, std::string_view, T*) { static_assert(sizeof(T) == 0); } // This should never be called
void bakeTo(const ghoul::Dictionary& d, std::string_view key, double* val) { *val = d.value<double>(key); }
void bakeTo(const ghoul::Dictionary& d, std::string_view key, std::string* val) { *val = d.value<std::string>(key); }

} // namespace internal

template <typename T> T bake(const ghoul::Dictionary&) { static_assert(sizeof(T) == 0); };
template <> Parameters bake<Parameters>(const ghoul::Dictionary& dict) {
    openspace::documentation::testSpecificationAndThrow(codegen::doc<openspace::KeplerTranslation>(), dict, "KeplerTranslation");
    Parameters res;
    internal::bakeTo(dict, "Eccentricity", &res.eccentricity);
    internal::bakeTo(dict, "SemiMajorAxis", &res.semiMajorAxis);
    internal::bakeTo(dict, "Inclination", &res.inclination);
    internal::bakeTo(dict, "AscendingNode", &res.ascendingNode);
    internal::bakeTo(dict, "ArgumentOfPeriapsis", &res.argumentOfPeriapsis);
    internal::bakeTo(dict, "MeanAnomaly", &res.meanAnomaly);
    internal::bakeTo(dict, "Epoch", &res.epoch);
    internal::bakeTo(dict, "Period", &res.period);
    return res;
}
} // namespace codegen
