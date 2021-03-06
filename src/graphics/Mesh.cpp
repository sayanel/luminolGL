//
// Created by mehdi on 16/02/16.
//

#include <glm/gtc/constants.hpp>
#include <fstream>
#include "graphics/Mesh.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <stdexcept>
#include <glm/ext.hpp>
#include <glog/logging.h>

namespace Graphics
{

    Mesh::Mesh() : _vertexCount(0), _triangleCount(0) { }
    Mesh::Mesh(Mesh &&mesh) :
            _vertexCount(mesh._vertexCount),
            _triangleCount(mesh._triangleCount),
            _vertices(std::move(mesh._vertices)),
            _elementIndex(std::move(mesh._elementIndex)),
            _boundaries(mesh._boundaries),
            _textures(std::move(mesh._textures))
    {}

    void Mesh::attachTexture(Graphics::Texture* tex, GLenum textureNumber) {
        _textures.insert({textureNumber, tex});
    }

    void Mesh::bindTextures() {
        for(auto& tex : _textures){
            tex.second->bind(tex.first);
        }
    }

    const std::vector<VertexDescriptor> &Mesh::getVertices() const {
        return _vertices;
    }


    const std::vector<int> &Mesh::getElementIndex() const {
        return _elementIndex;
    }


    int Mesh::getVertexCount() const {
        return _vertexCount;
    }

    int Mesh::getTriangleCount() const {
        return _triangleCount;
    }

    void Mesh::computeBoundingBox() {
        _boundaries.compute(_vertices);
    }

    Mesh Mesh::genCube() {
        Mesh mesh;

        mesh._vertices = {
                Graphics::VertexDescriptor(-0.5, -0.5, 0.5, 0, 0, 1, 0.f, 0.f),
                Graphics::VertexDescriptor(0.5, -0.5, 0.5, 0, 0, 1, 0.f, 1.f),
                Graphics::VertexDescriptor(-0.5, 0.5, 0.5, 0, 0, 1, 1.f, 0.f),
                Graphics::VertexDescriptor(0.5, 0.5, 0.5, 0, 0, 1, 1.f, 1.f),
                Graphics::VertexDescriptor(-0.5, 0.5, 0.5, 0, 1, 0, 0.f, 0.f),
                Graphics::VertexDescriptor(0.5, 0.5, 0.5, 0, 1, 0, 0.f, 1.f),
                Graphics::VertexDescriptor(-0.5, 0.5, -0.5, 0, 1, 0, 1.f, 0.f),
                Graphics::VertexDescriptor(0.5, 0.5, -0.5, 0, 1, 0, 1.f, 1.f),
                Graphics::VertexDescriptor(-0.5, 0.5, -0.5, 0, 0, -1, 0.f, 0.f),
                Graphics::VertexDescriptor(0.5, 0.5, -0.5, 0, 0, -1, 0.f, 1.f),
                Graphics::VertexDescriptor(-0.5, -0.5, -0.5, 0, 0, -1, 1.f, 0.f),
                Graphics::VertexDescriptor(0.5, -0.5, -0.5, 0, 0, -1, 1.f, 1.f),
                Graphics::VertexDescriptor(-0.5, -0.5, -0.5, 0, -1, 0, 0.f, 0.f),
                Graphics::VertexDescriptor(0.5, -0.5, -0.5, 0, -1, 0, 0.f, 1.f),
                Graphics::VertexDescriptor(-0.5, -0.5, 0.5, 0, -1, 0, 1.f, 0.f),
                Graphics::VertexDescriptor(0.5, -0.5, 0.5, 0, -1, 0, 1.f, 1.f),
                Graphics::VertexDescriptor(0.5, -0.5, 0.5, 1, 0, 0, 0.f, 0.f),
                Graphics::VertexDescriptor(0.5, -0.5, -0.5, 1, 0, 0, 0.f, 1.f),
                Graphics::VertexDescriptor(0.5, 0.5, 0.5, 1, 0, 0, 1.f, 0.f),
                Graphics::VertexDescriptor(0.5, 0.5, 0.5, 1, 0, 0, 1.f, 0.f),
                Graphics::VertexDescriptor(0.5, 0.5, -0.5, 1, 0, 0, 1.f, 1.f),
                Graphics::VertexDescriptor(-0.5, -0.5, -0.5, -1, 0, 0, 0.f, 1.f),
                Graphics::VertexDescriptor(-0.5, -0.5, 0.5, -1, 0, 0, 1.f, 1.f),
                Graphics::VertexDescriptor(-0.5, 0.5, -0.5, -1, 0, 0, 0.f, 0.f),
                Graphics::VertexDescriptor(-0.5, 0.5, -0.5, -1, 0, 0, 0.f, 0.f),
                Graphics::VertexDescriptor(-0.5, -0.5, 0.5, -1, 0, 0, 1.f, 1.f),
                Graphics::VertexDescriptor(-0.5, 0.5, 0.5, -1, 0, 0, 1.f, 0.f)
        };

        mesh._elementIndex = {
                0, 1, 2,
                2, 1, 3,
                4, 5, 6,
                6, 5, 7,
                8, 9, 10,
                10, 9, 11,
                12, 13, 14,
                14, 13, 15,
                16, 17, 18,
                19, 17, 20,
                21, 22, 23,
                24, 25, 26
        };

        mesh._triangleCount = 12;
        mesh._vertexCount = mesh._triangleCount * 3;

        mesh.computeBoundingBox();

        return mesh;
    }

    Mesh Mesh::genPlane(float width, float height, float textureLoop, const glm::vec3 & offset) {
        Mesh mesh;

        mesh._vertices = {
                Graphics::VertexDescriptor(-width/2 + offset.x, offset.y, height/2 + offset.z, 0, 1, 0, 0.f, 0.f),
                Graphics::VertexDescriptor(width/2 + offset.x, offset.y, height/2 + offset.z, 0, 1, 0, 0.f, textureLoop),
                Graphics::VertexDescriptor(-width/2 + offset.x, offset.y, -height/2 + offset.z, 0, 1, 0, textureLoop, 0.f),
                Graphics::VertexDescriptor(width/2 + offset.x, offset.y, -height/2 + offset.z, 0, 1, 0, textureLoop, textureLoop)
        };

        mesh._elementIndex = {
                0, 1, 2,
                2, 1, 3
        };

        mesh._triangleCount = 2;
        mesh._vertexCount = mesh._triangleCount * 3;

        mesh.computeBoundingBox();

        return mesh;
    }


    Mesh Mesh::genSphere(int latitudeBands, int longitudeBands, float radius, const glm::vec3 &offset) {
        Mesh mesh;

        std::vector<Graphics::VertexDescriptor> sphereVertices;
        for (int latNumber = 0; latNumber <= latitudeBands; latNumber++) {
            float theta = glm::pi<float>()*(float(latNumber) / float(latitudeBands));
            float sinTheta = glm::sin(theta);
            float cosTheta = glm::cos(theta);

            for (int longNumber = 0; longNumber <= longitudeBands; longNumber++) {
                float phi = glm::two_pi<float>() * (float(longNumber)  / float(longitudeBands));
                float sinPhi = glm::sin(phi);
                float cosPhi = glm::cos(phi);

                float x = sinPhi * sinTheta;
                float y = cosTheta;
                float z = cosPhi * sinTheta;

                float u = 1.f - (float(longNumber) / float(longitudeBands));
                float v = 1.f - (float(latNumber) / float(latitudeBands));

                sphereVertices.push_back(Graphics::VertexDescriptor(x*radius + offset.x, y * radius + offset.y, z * radius + offset.z, x, y, z, u, v));
            }
        }

        std::vector<int> sphereIds;
        for (int latNumber = 0; latNumber < latitudeBands; latNumber++) {
            for (int longNumber = 0; longNumber < longitudeBands; longNumber++) {
                int first = (latNumber * (longitudeBands + 1)) + longNumber;
                int second = first + longitudeBands + 1;
                sphereIds.push_back(first);
                sphereIds.push_back(second);
                sphereIds.push_back(first + 1);

                sphereIds.push_back(second);
                sphereIds.push_back(second + 1);
                sphereIds.push_back(first + 1);
            }
        }

        mesh.addVertices(sphereVertices);
        mesh.addElementIndexes(sphereIds);

        mesh._vertexCount = latitudeBands * longitudeBands * 6;
        mesh._triangleCount = mesh._vertexCount * 2;

        mesh.computeBoundingBox();

        return mesh;
    }


    Mesh Mesh::genGrid(int width, int height, const Texture* heightmap, glm::vec3 scale, float intensity, int smooth) {
        Mesh mesh;

        mesh._vertexCount = 0;
        mesh._triangleCount = 0;

        unsigned char* texData = heightmap ? heightmap->data() : nullptr;

        std::vector<Graphics::VertexDescriptor> gridVertices;
        float heightMean = 0;

        glm::vec3 offset(0.5f, 0, 0.5f);
        offset *= scale;

        DLOG(INFO) << "Vertices construction";

        // Vertices construction
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                float zValue = 0;

                if(heightmap){
                    int texI = int((i / float(height)) * float(heightmap->height()));
                    int texJ = int((j / float(width)) * float(heightmap->width()));

                    int padding = smooth;

                    float count = 0;
                    float mean = 0;
                    for(int x = -padding; x < padding; ++x){
                        for(int y = -padding; y < padding; ++y){
                            if( i + y < 0 || j + x < 0 ||  i + y > height || j + x > width){
                                if((i + y < 0 ||  i + y > height) && (j + x < 0 ||  j + x > width)){
                                    mean += texData[(texJ + (texI - y) * heightmap->height() - x)  * 3] / (255.f);
                                }
                                else if(i + y < 0 ||  i + y > height){
                                    mean += texData[(texJ + (texI - y) * heightmap->height() + x)  * 3] / (255.f);
                                }
                                else{
                                    mean += texData[(texJ + (texI + y) * heightmap->height() - x)  * 3] / (255.f);
                                }
                            }
                            else{
                                mean += texData[(texJ + (texI + y) * heightmap->height() + x) * 3] / (255.f);
                            }
                            count += 1.f;
                        }
                    }

                    mean /= count;
                    zValue = mean;

                }

                zValue *= intensity;
                glm::vec3 pos(j/float(width-1), zValue, i / float(height - 1));
                pos *= scale;
                pos -= offset;

                heightMean += pos.y;

                glm::vec3 norm(0,1,0);
                glm::vec2 uv(j/float(width-1), 1-i/float(height-1));
                gridVertices.push_back(VertexDescriptor(pos, norm, uv));
                ++mesh._vertexCount;
            }
        }

        heightMean /= gridVertices.size();

        for(auto& vert : gridVertices){
            vert.position.y -= heightMean;
        }

        DLOG(INFO) << "Normal & Ids generation";

        std::vector<int> gridIds;
        for (int i = 0; i < height - 1; ++i)
        {
            for (int j = 0; j < width - 1; ++j)
            {
                // ---  first triangle

                //insert ids
                int ip0 = (j + 0) + (i + 0) * height;
                int ip1 = (j + 1) + (i + 0) * height;
                int ip2 = (j + 0) + (i + 1) * height;
                gridIds.push_back(ip0);
                gridIds.push_back(ip1);
                gridIds.push_back(ip2);

                //compute new normal
                glm::vec3 v0 = gridVertices[ip1].position - gridVertices[ip0].position;
                glm::vec3 v1 = gridVertices[ip2].position - gridVertices[ip0].position;
                glm::vec3 newNormal = glm::normalize(glm::cross(v1,v0));
                gridVertices[ip0].normal = newNormal;
                gridVertices[ip1].normal = newNormal;
                gridVertices[ip2].normal = newNormal;

                // ---  second triangle
                //insert ids
                int ip3 = (j + 0) + (i + 1) * height;
                int ip4 = (j + 1) + (i + 0) * height;
                int ip5 = (j + 1) + (i + 1) * height;
                gridIds.push_back(ip3);
                gridIds.push_back(ip4);
                gridIds.push_back(ip5);

                //compute new normal
                v0 = gridVertices[ip4].position - gridVertices[ip3].position;
                v1 = gridVertices[ip5].position - gridVertices[ip3].position;
                newNormal = glm::normalize(glm::cross(v1, v0));
                gridVertices[ip3].normal = newNormal;
                gridVertices[ip4].normal = newNormal;
                gridVertices[ip5].normal = newNormal;

                mesh._triangleCount += 2;
            }
        }

        mesh.addVertices(gridVertices);
        mesh.addElementIndexes(gridIds);

        mesh.computeBoundingBox();

        return mesh;
    }

    const Geometry::BoundingBox &Mesh::getBoundingBox() {
        return _boundaries;
    }

    void Mesh::addVertices(const std::vector<VertexDescriptor> &vertices) {
        _vertices.insert(_vertices.end(), vertices.begin(), vertices.end());
    }

    void Mesh::addVertices(std::vector<VertexDescriptor> &&vertices) {
        _vertices.insert(_vertices.end(), std::make_move_iterator(vertices.begin()), std::make_move_iterator(vertices.end()));
    }

    void Mesh::addElementIndexes(const std::vector<int> &index) {
        _elementIndex.insert(_elementIndex.end(), index.begin(), index.end());
    }

    void Mesh::addElementIndexes(std::vector<int> &&index) {
        _elementIndex.insert(_elementIndex.end(), std::make_move_iterator(index.begin()), std::make_move_iterator(index.end()));
    }


    void Mesh::setTriangleCount(unsigned int value) {
        _triangleCount = value;
        _vertexCount = value * 3;
    }


    void Mesh::saveOBJ(const std::string &filePath, const std::string& filename, bool writeMTL) {

        // Write .obj
        std::ofstream objFile(filePath + filename + ".obj");

        if (!objFile.is_open())
            throw std::runtime_error("Unable to save mesh obj at \"" + filePath + "\"");

        objFile << "# " + filename + ".obj" << std::endl;
        objFile << "mtllib " + filename + ".mtl" << std::endl;

        // write positions
        for(unsigned int i = 0; i < _vertices.size(); ++i){
            objFile << "v ";
            objFile << _vertices[i].position.x << " ";
            objFile << _vertices[i].position.y << " ";
            objFile << _vertices[i].position.z << std::endl;
        }

        // write texcoords
        for(unsigned int i = 0; i < _vertices.size(); ++i){
            objFile << "vt ";
            objFile << _vertices[i].texcoord.x << " ";
            objFile << _vertices[i].texcoord.y << std::endl;
        }

        // write normals
        for(unsigned int i = 0; i < _vertices.size(); ++i){
            objFile << "vn ";
            objFile << _vertices[i].normal.x << " ";
            objFile << _vertices[i].normal.y << " ";
            objFile << _vertices[i].normal.z << std::endl;
        }

        objFile << "usemtl " + filename << std::endl;
        //write indexes
        for(unsigned int i = 0; i < _elementIndex.size(); i+=3){
            objFile << "f ";
            objFile << _elementIndex[i] + 1 << "/" << _elementIndex[i] + 1 << "/" << _elementIndex[i] + 1 << " ";
            objFile << _elementIndex[i + 1] + 1 << "/" << _elementIndex[i + 1] + 1 << "/" << _elementIndex[i + 1] + 1 << " ";
            objFile << _elementIndex[i + 2] + 1 << "/" << _elementIndex[i + 2] + 1 << "/" << _elementIndex[i + 2] + 1 << std::endl;
        }

        if(!writeMTL)
            return;

        // Write .mtl
        std::ofstream mtlFile(filePath + filename + ".mtl");

        if (!mtlFile.is_open())
            throw std::runtime_error("Unable to save mesh mtl at \"" + filePath + "\"");


        mtlFile << "#replace ext by your image extension " << std::endl;
        mtlFile << "newmtl " << filename << std::endl;
        mtlFile << "illum 2" << std::endl;
        mtlFile << "map_Kd " << filename << "_diff.ext" << std::endl;
        mtlFile << "map_Bump " << filename << "_normal.ext" << std::endl;
        mtlFile << "map_Ks " << filename << "_spec.ext" << std::endl;
    }

    std::map<GLenum, Texture *> &Mesh::textures() {
        return _textures;
    }

}