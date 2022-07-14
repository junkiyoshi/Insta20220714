#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(255);
	ofSetLineWidth(2);
	ofSetCircleResolution(60);
	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	ofSetColor(239, 39, 39);
	ofFill();
	ofDrawCircle(glm::vec3(0, 0, -1), 320);

	ofSetColor(39);
	ofNoFill();
	ofDrawCircle(glm::vec3(0, 0, -1), 320);

	auto head_size = 19;

	for (int k = 0; k < 2; k++) {

		ofMesh mesh;
		vector<glm::vec3> right, left, frame;
		auto noise_seed_deg = ofRandom(1000);
		auto noise_seed_radius = ofRandom(1000);

		glm::vec3 last_location;
		float last_theta, last_radius;

		for (int i = 0; i < 25; i++) {

			auto noise_deg = ofMap(ofNoise(noise_seed_deg, (ofGetFrameNum() + i) * 0.005), 0, 1, -360, 360);
			auto noise_radius = ofMap(ofNoise(noise_seed_radius, (ofGetFrameNum() + i) * 0.01), 0, 1, -350, 350);
			auto next_noise_deg = ofMap(ofNoise(noise_seed_deg, (ofGetFrameNum() + i + 1) * 0.005), 0, 1, -360, 360);
			auto next_noise_radius = ofMap(ofNoise(noise_seed_radius, (ofGetFrameNum() + i + 1) * 0.01), 0, 1, -350, 350);

			auto location = glm::vec3(noise_radius * cos(noise_deg * DEG_TO_RAD), noise_radius * sin(noise_deg * DEG_TO_RAD), 0);
			auto next = glm::vec3(next_noise_radius * cos(next_noise_deg * DEG_TO_RAD), next_noise_radius * sin(next_noise_deg * DEG_TO_RAD), 0);

			auto direction = next - location;
			auto theta = atan2(direction.y, direction.x);

			right.push_back(location + glm::vec3(ofMap(i, 0, 25, 0, head_size) * cos(theta + PI * 0.5), ofMap(i, 0, 25, 0, head_size) * sin(theta + PI * 0.5), 0));
			left.push_back(location + glm::vec3(ofMap(i, 0, 25, 0, head_size) * cos(theta - PI * 0.5), ofMap(i, 0, 25, 0, head_size) * sin(theta - PI * 0.5), 0));

			last_location = location;
			last_theta = theta;
			last_radius = noise_radius;
		}

		for (int i = 0; i < right.size(); i++) {

			mesh.addVertex(left[i]);
			mesh.addVertex(right[i]);
		}

		for (int i = 0; i < mesh.getNumVertices() - 2; i += 2) {

			mesh.addIndex(i + 0); mesh.addIndex(i + 1); mesh.addIndex(i + 3);
			mesh.addIndex(i + 0); mesh.addIndex(i + 2); mesh.addIndex(i + 3);
		}

		auto tmp_head_size = ofMap(mesh.getNumVertices() - 2, 0, mesh.getNumVertices(), 0, head_size);

		mesh.addVertex(last_location);
		int index = mesh.getNumVertices();
		for (auto theta = last_theta - PI * 0.5; theta <= last_theta + PI * 0.5; theta += PI / 20) {

			mesh.addVertex(last_location + glm::vec3(tmp_head_size * cos(theta), tmp_head_size * sin(theta), 0));
			frame.push_back(last_location + glm::vec3(tmp_head_size * cos(theta), tmp_head_size * sin(theta), 0));
		}

		for (int i = index; i < mesh.getNumVertices() - 1; i++) {

			mesh.addIndex(index); mesh.addIndex(i + 0); mesh.addIndex(i + 1);
		}

		k == 0 ? ofSetColor(255) : ofSetColor(0);

		int span = ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.003), 0, 1, 1, 7);
		int deg_span = 360 / span;
		for (int deg = 0; deg < 360; deg += 30) {

			ofRotate(deg_span);
			mesh.draw();
		}

		ofNoFill();
		ofDrawCircle(glm::vec3(), last_radius);
		ofPopMatrix();
	}
}


//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}