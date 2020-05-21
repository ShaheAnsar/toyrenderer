#undef TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <mesh.hpp>

//#ifdef MESH_DONE

namespace Rend{
  template<typename MaterialType, typename fn_mat>
  Mesh<MaterialType, fn_mat>::Mesh(std::string path, std::string base_dir, fn_mat func_mat) :
    vbo(0), vao(0), program_ids({}), texture_ids({}), ubo_tup_switches({}),
    ubo_tuples({}), mat_tuples({}), mats({}), success(false) {
    flog << "From Rend::Mesh" << std::endl;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;
    if (!tinyobj::LoadObj(&attrib, &shapes,
			  &materials, &warn, &err, path.c_str(),
			  base_dir.c_str(), true)) {
      flog << "[ERROR] Unable to load Model " << path << " :\n"
	   << err << std::endl;
    }
    else if (!warn.empty()) {
      flog << "[WARN] While loading model " << path << ":\n"
	   << warn << std::endl;
    } else if (!err.empty()) {
      flog << "[ERROR] Unable to load Model " << path << " :\n"
	   << err << std::endl;
    }
    flog << "[DEBUG] Loaded model " << path << " successfully" << std::endl;
    flog << "Material Info" << std::endl;
    for (auto &i : materials) {
      flog << "-----------------\n"
	   << "Material Name: " << i.name << "\n"
	   << "Ambient Color: <" << i.ambient[0] << "," << i.ambient[1] << ","
	   << i.ambient[2] << ">\n"
	   << "Diffuse Color: <" << i.diffuse[0] << "," << i.diffuse[1] << ","
	   << i.diffuse[2] << ">\n"
	   << "Specular Color: <" << i.specular[0] << "," << i.specular[1]
	   << "," << i.specular[2] << ">\n"
	   << "Ambient Texture: " << i.ambient_texname << "\n"
	   << "Diffuse Texture: " << i.diffuse_texname << "\n"
	   << "Specular Texture: " << i.specular_texname << std::endl;
    }
    flog << "[MODEL] Shapes: " + std::to_string( shapes.size() ) << "\n";
    flog << "[MODEL] materials: " + std::to_string( materials.size() ) << std::endl;
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    std::vector<float> vbo_data;
    int current_material_id = -1;
    for(std::size_t i = 0; i < shapes.size(); i++) {
      for(std::size_t mesh_i = 0; mesh_i < shapes[i].mesh.indices.size(); mesh_i++) {
	tinyobj::index_t idx = shapes[i].mesh.indices[mesh_i];
	vbo_data.push_back(attrib.vertices[3*idx.vertex_index + 0]);
	vbo_data.push_back(attrib.vertices[3*idx.vertex_index + 1]);
	vbo_data.push_back(attrib.vertices[3*idx.vertex_index + 2]);
	vbo_data.push_back(attrib.normals[3*idx.normal_index + 0]);
	vbo_data.push_back(attrib.normals[3*idx.normal_index + 1]);
	vbo_data.push_back(attrib.normals[3*idx.normal_index + 2]);
        if (!attrib.texcoords.empty()) {
          vbo_data.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
          vbo_data.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
        }
      }
      for(std::size_t face_i = 0; face_i < shapes[i].mesh.num_face_vertices.size(); face_i++) {
	if(mat_tuples.empty()) {
	  mat_tuples.push_back({});
	  std::get<0>(mat_tuples.back()) = shapes[i].mesh.material_ids[face_i];
	  std::get<1>(mat_tuples.back()) = face_i;
	  std::get<2>(mat_tuples.back()) = 1;
	  current_material_id = shapes[i].mesh.material_ids[face_i];
	  continue;
	}
	if(current_material_id == shapes[i].mesh.material_ids[face_i]) {
	  std::get<2>( mat_tuples.back() )++;
	}
	else {
	  mat_tuples.push_back({});
	  std::get<0>(mat_tuples.back()) = shapes[i].mesh.material_ids[face_i];
	  std::get<1>(mat_tuples.back()) = std::get<1>(mat_tuples[mat_tuples.size() - 2]) + std::get<2>(mat_tuples[mat_tuples.size() - 2]);
	  std::get<2>(mat_tuples.back()) = 1;
	  current_material_id = shapes[i].mesh.material_ids[face_i];
	}
      }
    }

    flog << "VBO size/8 " + std::to_string(vbo_data.size()/8);
    glBufferData(GL_ARRAY_BUFFER, vbo_data.size()*sizeof(float), vbo_data.data(), GL_STREAM_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    for(std::size_t i = 0; i < materials.size(); i++) {
      mats.push_back({});
      func_mat(mats.back(), materials[i]);
    }

    success = true;
    flog << "Materials: " << std::to_string(mat_tuples.size()) << std::endl;
  }
}
