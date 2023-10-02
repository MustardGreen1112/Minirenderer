#include "standard_fragment_shader.h"

/// <summary>
/// If fragment is in shadow, return 0, else, return 1.
/// </summary>
/// <param name="shadow_map"></param>
/// <param name="lightSpacePosition"></param>
/// <param name="light2clip"></param>
/// <param name="bias"></param>
/// <returns></returns>
float get_shadow_value(texture& shadow_map, vector3 lightSpacePosition, mat4x4 light2clip, float bias) {
	vector4 lightSpacePosition_4 = vector3_to_4(lightSpacePosition);
	lightSpacePosition_4.w = 1.0f;
	lightSpacePosition = vector4_to_3(mul(light2clip, lightSpacePosition_4));
	vector2 shadowmap_uv = vector2{ 0.5f * lightSpacePosition.x + 0.5f,0.5f * lightSpacePosition.y + 0.5f };
	float lightSpaceDepth = 0.5f * lightSpacePosition.z + 0.5f;
	float shadowmap_depth = shadow_map.sample2D(shadowmap_uv).r;
	return shadowmap_depth - lightSpaceDepth >= -bias ? 1.0f:0;
}

vector4 standard_fragment_shader(shader_context& input, const void* uniform, const material& mat) {

	vector3 world_position = input.read_data_v3(WORLD_SPACE_POSITION);
	vector3 world_normal = normalize(input.read_data_v3(WORLD_SPACE_NORMAL));
	vector4 world_tangent_v4 = input.read_data_v4(WORLD_SPACE_TANGENT);
	vector3 world_tangent = normalize(vector4_to_3(world_tangent_v4));
	//Transform uv.
	vector2 uv = input.read_data_v2(TEXCOORD);
	vector4 uvtransform = mat.GetUVTransform();
	uv = vector2{ uv.u * uvtransform.x + uvtransform.z, uv.v * uvtransform.y + uvtransform.w };
	//Construct tangent to world matrix.
	float tangent_w = world_tangent.w;
	vector3 world_binormal = normalize(cross(world_tangent, world_normal) * tangent_w);
	mat3x3 tangent_to_world = transpose(mat3x3{ world_tangent, world_binormal, world_normal });

	//Phong shading(Per pixel shading). Blinn-Phong reflection model. 
	standard_uniform unif = *(standard_uniform*)uniform;
	vector3 viewDir = normalize(unif.camera_position - world_position);
	vector4 ambient = unif.ambientColor;
	ambient = { ambient.x * ambient.w, ambient.y * ambient.w, ambient.z * ambient.w, 1.0f };
	//Calculate normal.(Using tangent space).
	texture* normalTex = mat.GetNormalTexture();
	if (normalTex != nullptr) {
		vector3 tangent_normal = vector4_to_3(normalTex->sample2D(uv));
		tangent_normal = vector3{ tangent_normal.x * 2.0f - 1.0f, tangent_normal.y * 2.0f - 1.0f, tangent_normal.z * 2.0f -1.0f };
		world_normal = normalize(mul(tangent_to_world, tangent_normal));
	}
	//Calculate diffuse.(Lambert Law).
	vector3 lightDir = (unif.light_direction) * -1.0f;
	float diffuse = 0.5f * dot(lightDir, world_normal) + 0.5f;
	vector4 albedo = mat.GetColor();
	//Calculate diffuse texture color.
	texture* diffuseTex = mat.GetDiffuseTexutre();
	if (diffuseTex != nullptr) {
		vector4 dif = diffuseTex->sample2D(uv);
		albedo = vector4{ dif.r * albedo.r, dif.g * albedo.g, dif.b * albedo.b, 1.0f } * diffuse;
		albedo.w = 1.0f;
	}
	//Calculate specular.(Blinn model, half direction).
	vector3 halfDir = normalize(lightDir + viewDir);
	float spec = powf(float_clamp01(dot(halfDir, world_normal)), mat.GetGlossValue()* 9.0f + 1.0f);
	texture* specularTex = mat.GetSpecularTexture();
	if (specularTex != nullptr) {
		spec *= specularTex->sample2D(uv).r;
	}
	vector4 specular = vector4{ spec, spec, spec, 1.0f };
	//Calculate shadow.
	vector3 lightSpacePos = input.read_data_v3(LIGHT_SPACE_POSITION);
	float shadowVal = get_shadow_value(*unif.shadowmap, lightSpacePos, unif.light2clip, 0.001f);
	shadowVal = shadowVal > 0? 1.0f:0.32f;
	//Shift shadow to blue tune.
	vector4 shadow = vector4{ unif.ambientColor.r,unif.ambientColor.g, unif.ambientColor.b, 1.0f } *shadowVal;
	vector4 result = (albedo + specular);
	result = vector4{ result.r * shadow.r, result.g * shadow.g, result.b * shadow.b, 1.0f };
	result = result + ambient;
	result.w = 1.0f;

	return result;
}