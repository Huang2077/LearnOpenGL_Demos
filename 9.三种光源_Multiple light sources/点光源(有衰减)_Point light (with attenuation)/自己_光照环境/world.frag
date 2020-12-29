#version 330 core//片段着色器代码

struct Material//物体材质：自己本身颜色、镜面反射强度、散射半径
{
sampler2D  diffuse;  //物体漫反射的颜色（物体本质颜色）
sampler2D specular; //镜面反射是不是很亮
float shininess;//散射半径
};

struct Light //光的属性
{
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
float Kc;
float Kl;
float Kq;
};

in vec3 Normal;//法向量
in vec3 FragPos;//片段位置
in vec2 TexCoords;

out vec4 color;
uniform vec3 cameraPos;
uniform Material material;
uniform Light light;

void main()
    {
	
    //全局光照
    vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));//全局光照
	//漫反射分量
	vec3 norm = normalize(Normal);//将法向量单位化
	vec3 lightDir = normalize( light.position - FragPos);//将光找到片段上的方向单位化
	float diff = max(dot(norm,lightDir) , 0.0);//漫反射因子
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse,TexCoords));//漫反射光分量
	//镜面反射
	vec3 viewDir = normalize(cameraPos - FragPos);//片段与玩家视角所成角度
	vec3 reflectDir = reflect(-lightDir, norm);//经过反射后的光角度
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess );//镜面反射因子，32是物体的发光值，发光值（光滑度）越高反射能力越强
	vec3 specular = vec3(texture(material.specular,TexCoords)) * spec * light.specular;//镜面反射光分量
	//计算衰减
	float distance = length(light.position - FragPos); //光到片段距离
	float attenuation = 1.0f/(light.Kc + light.Kl*distance + light.Kq*(distance*distance));//衰减公式（最后所剩百分比）

	vec3 result = (diffuse + ambient + specular) * attenuation;//最终结果

    color = vec4(result, 1.0f);
    }