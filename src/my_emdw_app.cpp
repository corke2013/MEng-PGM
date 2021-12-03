// patrec headers
#include <prlite_logging.hpp>
#include <prlite_testing.hpp>
#include <prlite_genmat.hpp>
#include <prlite_stlvecs.hpp>

// emdw headers
#include <emdw.hpp>
#include <discretetable.hpp>
#include <clustergraph.hpp>
#include <lbp_cg.hpp>
#include <lbu_cg.hpp>
#include <lbu2_cg.hpp>
#include <sigmapoints.hpp>

// standard headers
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <memory>
#include <set>
#include <map>
#include <algorithm>
#include <limits>
#include <chrono>
#include <iomanip>

//custom headers
#include <cam_dist.hpp>
#include <preprocessing.hpp>
#include <json.hpp>
#include <vel_model.hpp>
#include <dis_model.hpp>

using namespace std;
using namespace emdw;
using json = nlohmann::json;

int main() {
	std::vector<rcptr<Factor>> factors;
	std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>, double>> modelParams{
		{"B", 0.6},
		{"staticMean", prlite::vector2ColVector(vector<double> {0, 0, 0})},
		{"staticCov", Utils::vector2DToRowMatrix(vector<vector<double>> {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}})},
		{"dynamicMean", prlite::vector2ColVector(vector<double> {0, 0, 0})},
		{"dynamicCov", Utils::vector2DToRowMatrix(vector<vector<double>> {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}})}
	};

	ifstream ifsIdxFeatIdVel("idx_feat_id_vel.json");
	ifstream ifsIdxFeatIdCam("idx_feat_id_cam.json");
	json idxFeatIdVelJsonData = json::parse(ifsIdxFeatIdVel);
	json idxFeatIdCamJsonData = json::parse(ifsIdxFeatIdCam);

	double maxDistance = 3;
	Preprocessing preprocessing;
	std::map<unsigned int, std::map<unsigned int, std::map<std::string, std::variant<gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>>> idxFeatIdVelData = preprocessing.getIdxFeatIdVelData(idxFeatIdVelJsonData);
	std::map<unsigned int, std::map<std::size_t, std::tuple<unsigned int, unsigned int, gLinear::gColVector<double>, gLinear::gRowMatrix<double>>>> idxHashDisData = preprocessing.getIdxHashDisData(maxDistance, idxFeatIdCamJsonData);

	VelModel velModel(&factors, modelParams);
	DisModel disModel(&factors, modelParams);
	
	for (unsigned int i = 0; i < 109; i++) {
		if (idxFeatIdVelData.find(i) != idxFeatIdVelData.end()) {
			velModel.buildVelModel(i, idxFeatIdVelData[i]);
		}
		if (idxHashDisData.find(i) != idxHashDisData.end()) {
			disModel.buildDisModel(i, idxHashDisData[i]);
		}

		cerr << "We have " << factors.size() << " factors here\n";
	}
	usleep(1000000);
}