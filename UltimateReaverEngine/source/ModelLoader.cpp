#include "ModelLoader.h"
#include <fstream>
#include <sstream>
#include <map>
#include <Windows.h>

static bool fileExistsA(const char* p) {
  DWORD a = GetFileAttributesA(p);
  return a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY);
}

static bool safeNextInt(std::stringstream& ss, int& out) {
  std::string tok;
  if (!std::getline(ss, tok, '/')) return false;
  if (tok.empty()) return false;
  try { out = std::stoi(tok); }
  catch (...) { return false; }
  return true;
}

bool
ModelLoader::loadModel(const std::string& fileName, MeshComponent& outMesh) {
  // 0) validar existencia del archivo (NO fallback)
  if (!fileExistsA(fileName.c_str())) {
    OutputDebugStringA(("[ModelLoader] Not found: " + fileName + "\n").c_str());
    return false;
  }

  std::ifstream file(fileName);
  if (!file.is_open()) {
    ERROR("ModelLoader.cpp", "loadModel", "The file couldn't be opened.");
    return false;
  }

  // limpiar destino
  outMesh.m_vertex.clear();
  outMesh.m_index.clear();
  outMesh.m_numVertex = 0;
  outMesh.m_numIndex = 0;

  std::string line;
  std::vector<XMFLOAT3> tempPos;
  std::vector<XMFLOAT2> tempUV;
  std::vector<XMFLOAT3> tempNrm;
  std::map<std::string, int> uniqueVertex;

  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;

    std::stringstream ls(line);
    std::string tag;
    ls >> tag;

    if (tag == "v") {
      XMFLOAT3 p; ls >> p.x >> p.y >> p.z; tempPos.push_back(p);
    }
    else if (tag == "vt") {
      XMFLOAT2 uv; ls >> uv.x >> uv.y;     tempUV.push_back(uv);
    }
    else if (tag == "vn") {
      XMFLOAT3 n; ls >> n.x >> n.y >> n.z; tempNrm.push_back(n);
    }
    else if (tag == "f") {
      // recolectar tokens de la cara: v/t/n | v//n | v/t
      std::vector<int> faceIdx;

      std::string chunk;
      while (ls >> chunk) {
        int finalIndex = 0;

        auto it = uniqueVertex.find(chunk);
        if (it != uniqueVertex.end()) {
          finalIndex = it->second;
        }
        else {
          // parsear chunk
          // soporta: v/t/n, v//n, v/t
          int posIdx = -1, texIdx = -1, nrmIdx = -1;

          std::stringstream vs(chunk);
          std::string first;
          std::getline(vs, first, '/');
          if (!first.empty()) { try { posIdx = std::stoi(first) - 1; } catch (...) { posIdx = -1; } }

          if (vs.peek() == '/') { // caso v//n
            vs.get(); // consume '/'
            std::string third;
            std::getline(vs, third, '/');
            if (!third.empty()) { try { nrmIdx = std::stoi(third) - 1; } catch (...) { nrmIdx = -1; } }
          }
          else {
            // tenemos al menos v/...
            std::string second;
            std::getline(vs, second, '/'); // uv
            if (!second.empty()) { try { texIdx = std::stoi(second) - 1; } catch (...) { texIdx = -1; } }

            std::string third;
            if (std::getline(vs, third, '/')) {
              if (!third.empty()) { try { nrmIdx = std::stoi(third) - 1; } catch (...) { nrmIdx = -1; } }
            }
          }

          // validar índices
          if (posIdx < 0 || posIdx >= (int)tempPos.size()) {
            OutputDebugStringA("[ModelLoader] Bad position index in face.\n");
            file.close();
            return false; // NO fallback
          }

          XMFLOAT3 P = tempPos[posIdx];
          XMFLOAT2 T = { 0.f, 0.f };
          if (texIdx >= 0 && texIdx < (int)tempUV.size()) T = tempUV[texIdx];

          // construir vértice
          SimpleVertex v{};
          v.Pos = P;
          v.Tex = T;
          // si agregas normales a tu Vertex/Shader, aquí asignas v.Norm

          outMesh.m_vertex.push_back(v);
          finalIndex = (int)outMesh.m_vertex.size() - 1;
          uniqueVertex[chunk] = finalIndex;
        }

        faceIdx.push_back(finalIndex);
      }

      // triangulación en abanico (n-gon -> tri-list)
      if (faceIdx.size() < 3) {
        OutputDebugStringA("[ModelLoader] face with <3 vertices ignored.\n");
        continue;
      }
      for (size_t i = 1; i + 1 < faceIdx.size(); ++i) {
        outMesh.m_index.push_back(faceIdx[0]);
        outMesh.m_index.push_back(faceIdx[i]);
        outMesh.m_index.push_back(faceIdx[i + 1]);
      }
    }
  }

  outMesh.m_numVertex = (int)outMesh.m_vertex.size();
  outMesh.m_numIndex = (int)outMesh.m_index.size();

  file.close();

  // Log rápido
  char tmp[128];
  sprintf_s(tmp, "[ModelLoader] %s -> Verts:%d Indices:%d\n",
    fileName.c_str(), outMesh.m_numVertex, outMesh.m_numIndex);
  OutputDebugStringA(tmp);

  // si no cargó nada, falla explícito
  if (outMesh.m_numVertex == 0 || outMesh.m_numIndex == 0) {
    OutputDebugStringA("[ModelLoader] Empty mesh -> fail (NO fallback cube)\n");
    return false;
  }

  return true;
}

void
ModelLoader::parseVec2(std::stringstream& streamLine, std::vector<XMFLOAT2>& outVector) {
  XMFLOAT2 uv{};
  streamLine >> uv.x >> uv.y;
  outVector.push_back(uv);
}

void
ModelLoader::parseVec3(std::stringstream& streamLine, std::vector<XMFLOAT3>& outVector) {
  XMFLOAT3 v{};
  streamLine >> v.x >> v.y >> v.z;
  outVector.push_back(v);
}
