#include <utils.hpp>

// tested
std::size_t Utils::hashFromInts(unsigned int a, unsigned int b) {
	std::hash<std::string> stringHasher;
	return stringHasher(std::to_string(a) + ":" + std::to_string(b));
}
 
// tested
std::size_t Utils::hashFromInts(unsigned int a, unsigned int b, unsigned int c) {
	std::hash<std::string> stringHasher;
	return stringHasher(std::to_string(a) + ":" + std::to_string(b) + ":" + std::to_string(c));
}

// tested
std::size_t Utils::hashFromInts(unsigned int a, unsigned int b, unsigned int c, unsigned int d) {
	std::hash<std::string> stringHasher;
	return stringHasher(std::to_string(a) + ":" + std::to_string(b) + ":" + std::to_string(c) + ":" + std::to_string(d));
}

// tested
std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>> Utils::jsonDataToMapData(nlohmann::json jsonData) {
	std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>> mapData;
	for (auto i = jsonData.begin(); i != jsonData.end(); i++) {
		for (auto j = i.value().begin(); j != i.value().end(); j++) {
			mapData[std::stoi(i.key())][std::stoi(j.key())]["mean"] = prlite::vector2ColVector(j.value()["mean"].get<std::vector<double>>());
			mapData[std::stoi(i.key())][std::stoi(j.key())]["cov"] = Utils::vector2DToRowMatrix(j.value()["cov"].get<std::vector<std::vector<double>>>());
		}
	}
	return mapData;
}

// tested
gLinear::gRowMatrix<double> Utils::vector2DToRowMatrix(std::vector<std::vector<double>> vector2D) {
	unsigned int r = vector2D.size();
	unsigned int c = vector2D[0].size();
	gLinear::gRowMatrix<double> rowMatrix(r, c);

	for (unsigned int i = 0; i < r; i++) {
		rowMatrix[i] = prlite::vector2Vector(vector2D[i]);
	}
	return rowMatrix;
}

// not tested
std::vector<std::vector<double>> Utils::rowMatrixTo2DVector(gLinear::gMatrix<double, gLinear::ROW_DENSE> rowMatrix) {
	unsigned int r = rowMatrix.rows();
	std::vector<std::vector<double>> vector2D;
	for (unsigned int i = 0; i < r; i++) {
		vector2D.push_back(prlite::Vector2vector(rowMatrix[i]));
	}
	return vector2D;
}

// not tested
double Utils::getDistance(double a, double b, double c, double d, double e, double f) {
	double distance = std::sqrt(std::pow(a - b, 2) + std::pow(c - d, 2) + std::pow(e - f, 2));
	return distance;
}

// not tested
double Utils::getLikelyhood(gLinear::gColVector<double> measMean, gLinear::gRowMatrix<double> measCov, gLinear::gColVector<double> modelMean, gLinear::gRowMatrix<double> modelCov) {
	int fail;
	gLinear::gColVector<double> xu = measMean - modelMean;
	gLinear::gRowVector<double> xuT = prlite::vector2Vector(prlite::ColVector2vector(xu));
	gLinear::gMatrix<double> covMatrix = measCov + modelCov;

	double det = gLinear::det((2 * M_PI * covMatrix), fail);
	if (fail) {
		throw std::exception();
	}
	gLinear::gMatrix<double> inv = gLinear::inv(covMatrix, fail);
	if (fail) {
		throw std::exception();
	}

	return std::pow(det, -0.5) * std::pow(M_E, (-0.5 * xuT * inv * xu));
}
