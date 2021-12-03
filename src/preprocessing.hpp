#ifndef PREPROCESSING_HPP
#define PREPROCESSING_HPP

//std headers
#include <map>
#include <vector>
#include <variant>

//custom headers
#include <json.hpp>
#include <utils.hpp>
#include <cam_dist.hpp>

class Preprocessing
{
public:
	Preprocessing();
	std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>> getIdxFeatIdVelData(nlohmann::json featIdIdx);
	std::map<unsigned int, std::map<std::size_t, std::tuple<unsigned int, unsigned int, gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>> getIdxHashDisData(double maxDistance, nlohmann::json idxFeatIdCamJsonData);
private:
	std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>> getFeatIdIdxVelMapData(nlohmann::json idxFeatIdJsonData);
	std::map<std::size_t, std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>>> getHashIdxValidConnections(double maxDistance, nlohmann::json idxFeatIdCamJsonData);
};
#endif // !PREPROCESSING_HPP