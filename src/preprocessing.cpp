// custom headers
#include <preprocessing.hpp>

Preprocessing::Preprocessing() {

}

std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>> Preprocessing::getFeatIdIdxVelMapData(nlohmann::json idxFeatIdVelJsonData) {
	std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>> idxFeatIdVelMapData = Utils::jsonDataToMapData(idxFeatIdVelJsonData);
	std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>> featIdIdxVelMapData;
	for (auto [idx, featIdVelMapData] : idxFeatIdVelMapData) {
		for (auto [featId, velMapData] : featIdVelMapData) {
			// [featId]		Feature id
			// [idx]		Time step
			featIdIdxVelMapData[featId][idx] = velMapData;
		}
	}
	return featIdIdxVelMapData;
}

std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>> Preprocessing::getIdxFeatIdVelData(nlohmann::json idxFeatIdVelJsonData) {
	std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>> featIdIdxVelMapData = getFeatIdIdxVelMapData(idxFeatIdVelJsonData);
	std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>> idxFeatIdVelData;
	for (auto [featId, idxVelMapData] : featIdIdxVelMapData) {
		if (std::size(idxVelMapData) < 2) continue;
		for (auto i = idxVelMapData.begin(); i != std::prev(idxVelMapData.end()); i++) {
			// [i->first]	Time step
			// [featId]		Feature id
			gLinear::gColVector<double> mean0 = std::get<gLinear::gColVector<double>>(i->second["mean"]);
			gLinear::gRowMatrix<double> cov0 = std::get<gLinear::gRowMatrix<double>>(i->second["cov"]);
			gLinear::gColVector<double> mean1 = std::get<gLinear::gColVector<double>>(std::next(i)->second["mean"]);
			gLinear::gRowMatrix<double> cov1 = std::get<gLinear::gRowMatrix<double>>(std::next(i)->second["cov"]);
			idxFeatIdVelData[i->first][featId] = {
				{"mean", (mean1 - mean0) / (std::next(i)->first - i->first)},
				{"cov", (cov1 + cov0) * double(std::pow((std::next(i)->first - i->first), 2))}
			};
		}
	}
	//for (auto i = featIdIdxVelMapData.begin(); i != featIdIdxVelMapData.end(); i++) {
	//	if (std::size(i->second) < 2) continue;
	//	for (auto j = i->second.begin(); j != std::prev(i->second.end()); j++) {
	//		std::vector<double> mean0 = std::get<std::vector<double>>(j->second["mean"]);
	//		std::vector<std::vector<double>> cov0 = std::get <std::vector<std::vector<double>>>(j->second["cov"]);
	//		std::vector<double> mean1 = std::get<std::vector<double>>(std::next(j)->second["mean"]);
	//		std::vector<std::vector<double>> cov1 = std::get <std::vector<std::vector<double>>>(std::next(j)->second["cov"]);
	//		idxFeatIdVelData[j->first][i->first] = { {"mean", prlite::Vector2vector((prlite::vector2Vector(mean1) - prlite::vector2Vector(mean0)) / (std::next(j)->first - j->first))}, {"cov", Utils::matrixto2DVector(Utils::vector2DToMatrix(cov1) + Utils::vector2DToMatrix(cov0))} };
	//	}
	//}
	return idxFeatIdVelData;
}

std::map<std::size_t, std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>>> Preprocessing::getHashIdxValidConnections(double maxDistance, nlohmann::json idxFeatIdCamJsonData) {
	std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>> idxFeatIdCamMapData = Utils::jsonDataToMapData(idxFeatIdCamJsonData);
	std::map<std::size_t, std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>>> hashIdxValidConnections;
	for (auto [idx, featIdCamMapData] : idxFeatIdCamMapData) {
		for (auto i = featIdCamMapData.begin(); i != featIdCamMapData.end(); i++) {
			for (auto j = std::next(i); j != featIdCamMapData.end(); j++) {
				// [hash]		Hash
				// [idx]		Time step
				// [i->first]	Key i
				// [j->first]	Key j
				gLinear::gColVector<double> mean0 = std::get<gLinear::gColVector<double>>(i->second["mean"]);
				gLinear::gColVector<double> mean1 = std::get<gLinear::gColVector<double>>(j->second["mean"]);
				if (Utils::getDistance(mean0[0], mean1[0], mean0[1], mean1[1], mean0[2], mean1[2]) >= maxDistance) continue;
				std::size_t hash = Utils::hashFromInts(i->first, j->first);
				hashIdxValidConnections[hash][idx][i->first] = { {"mean", mean0}, {"cov", std::get<gLinear::gRowMatrix<double>>(i->second["cov"])} };
				hashIdxValidConnections[hash][idx][j->first] = { {"mean", mean1}, {"cov", std::get<gLinear::gRowMatrix<double>>(j->second["cov"])} };
			}
		}
	}
	//for (auto i = idxFeatIdCamMapData.begin(); i != idxFeatIdCamMapData.end(); i++) {
	//	for (auto j = i->second.begin(); j != i->second.end(); j++) {
	//		for (auto k = std::next(j); k != i->second.end(); k++) {
	//			// [hash]		Hash
	//			// [i->first]	Time step
	//			// [j->first]	Key i
	//			// [k->first]	Key j
	//			std::vector<double> mean0 = std::get<std::vector<double>>(j->second["mean"]);
	//			std::vector<double> mean1 = std::get<std::vector<double>>(k->second["mean"]);
	//			if (Utils::getDistance(mean0[0], mean1[0], mean0[1], mean1[1], mean0[2], mean1[2]) >= maxDistance) continue;
	//			std::size_t hash = Utils::hashFromInts(j->first, k->first);
	//			hashIdxFeatIdValidConnections[hash][i->first][j->first] = {{"mean", mean0}, {"cov", std::get<std::vector<std::vector<double>>>(j->second["cov"])}};
	//			hashIdxFeatIdValidConnections[hash][i->first][k->first] = {{"mean", mean1}, {"cov", std::get<std::vector<std::vector<double>>>(k->second["cov"])}};
	//		}
	//	}
	//}
	return hashIdxValidConnections;
}

std::map<unsigned int, std::map<std::size_t, std::tuple<unsigned int, unsigned int, gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>> Preprocessing::getIdxHashDisData(double maxDistance, nlohmann::json idxFeatIdCamJsonData) {
	unsigned dim = 12;
	double w0 = 0.5 / (2.0 * dim + 1);
	std::map<std::size_t, std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>>> hashIdxValidConnections = getHashIdxValidConnections(maxDistance, idxFeatIdCamJsonData);
	std::map<unsigned int, std::map<std::size_t, std::tuple<unsigned int, unsigned int, gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>> idxHashDisData;
	for (auto [hash, idxValidConnections] : hashIdxValidConnections) {
		if (std::size(idxValidConnections) < 2) continue;
		for (auto i = idxValidConnections.begin(); i != std::prev(idxValidConnections.end()); i++) {
			// [i->first]	Time step
			// [hash]		Hash
			std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xi0 = i->second[i->second.begin()->first];
			std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xj0 = i->second[std::next(i->second.begin())->first];
			std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xi1 = std::next(i)->second[i->second.begin()->first];
			std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>> xj1 = std::next(i)->second[std::next(i->second.begin())->first];
			std::tuple<gLinear::gColVector<double>, gLinear::gRowMatrix<double>> displacementLinearisedDistribution = getDisplacementLinearisedDistribution(w0, xi0, xj0, xi1, xj1);
			idxHashDisData[i->first][hash] = std::make_tuple(i->second.begin()->first, std::next(i->second.begin())->first, std::get<0>(displacementLinearisedDistribution), std::get<1>(displacementLinearisedDistribution) * double(std::next(i)->first - i->first));
		}
	}
	//for (auto i = hashIdxValidConnections.begin(); i != hashIdxValidConnections.end(); i++) {
	//	if (std::size(i->second) < 2) continue;
	//	for (auto j = i->second.begin(); j != std::prev(i->second.end()); j++) {
	//		// [j->first]	Time step
	//		// [i->first]	Hash
	//		std::map<std::string, std::variant<std::vector<double>, std::vector<std::vector<double>>>> xi0 = j->second[j->second.begin()->first];
	//		std::map<std::string, std::variant<std::vector<double>, std::vector<std::vector<double>>>> xj0 = j->second[std::next(j->second.begin())->first];
	//		std::map<std::string, std::variant<std::vector<double>, std::vector<std::vector<double>>>> xi1 = std::next(j)->second[j->second.begin()->first];
	//		std::map<std::string, std::variant<std::vector<double>, std::vector<std::vector<double>>>> xj1 = std::next(j)->second[std::next(j->second.begin())->first];
	//		std::tuple<prlite::ColVector<double>, prlite::RowMatrix<double>> displacementLinearisedDistribution = getDisplacementLinearisedDistribution(w0, xi0, xj0, xi1, xj1);
	//		idxHashDisData[j->first][i->first] = std::make_tuple(j->second.begin()->first, std::next(j->second.begin())->first, std::get<0>(displacementLinearisedDistribution), std::get<1>(displacementLinearisedDistribution));
	//	}
	//}
	return idxHashDisData;
}