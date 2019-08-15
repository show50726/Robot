#include "main.h"
float degree = 90;
static int oldPosY = -1;
static int oldPosX = -1;
vec3 camera = vec3(0,0,20);
int gammaok = 0;
GLuint loc;

void changeViewPoint(int x, int y)
{
	int temp = x - oldPosX;
	degree -= temp * 0.05f;
	oldPosX = x;
	oldPosY = y;
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitContextVersion(4,3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);


	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(600, 680);
	glutCreateWindow("Robot");

	glewExperimental = GL_TRUE; 
	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(Keyboard);
	int ActionMenu,ModeMenu,ShaderMenu;
	ActionMenu = glutCreateMenu(ActionMenuEvents);

	glutAddMenuEntry("idle",0);
	glutAddMenuEntry("walk",1);
	glutAddMenuEntry("jump", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	

	ModeMenu = glutCreateMenu(ModeMenuEvents);

	glutAddMenuEntry("Line",0);
	glutAddMenuEntry("Fill",1);
	glutAddMenuEntry("Gray",2);
	glutAddMenuEntry("Negative", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	glutCreateMenu(menuEvents);

	glutAddSubMenu("action",ActionMenu);
	glutAddSubMenu("mode",ModeMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	
	glutMotionFunc(changeViewPoint);
	glutMouseFunc(Mouse);
	glutTimerFunc (100, idle, 0); 

	glutMainLoop();
	return 0;
}


void ChangeSize(int w,int h){
	if(h == 0) h = 1;
	glViewport(0,0,w,h);
	Projection = perspective(80.0f,(float)w/h,0.1f,100.0f);
}
void Mouse(int button,int state,int x,int y){
	if (state == GLUT_DOWN) {
		oldPosX = x; oldPosY = y;
	}
	if(button == 2) isFrame = false;
}
void idle(int dummy){
	isFrame = true;
	int out = 0;
	if(action == WALK){
		updateObj(dummy);
		out = dummy+1;
		if(out > 12) out = 1;
	}
	else if (action == DANCE) {
		dance(dummy);
		out = dummy + 1;
		if (out > 12) out = 1;
	}
	else if(action == IDLE){
		resetObj(dummy);
		out = 0;
	}
	glutPostRedisplay();
	
	glutTimerFunc (150, idle, out); 
}

void dance(int frame) {
	switch (frame) {
	case 0:
		//左手
		//angles[1] = -10;
		//右手

		//腿
		anglesg[12] = -10;

		break;
	case 1:
	case 2:
	case 3:
		anglesg[4] += 30;
		//anglesg[1] += 10;
		anglesg[12] += 10;
		position += 0.2;
		break;
	case 4:
	case 5:
	case 6:
		anglesg[4] += 30;
		//anglesg[1] += 10;
		anglesg[12] += 10;
		//angles[13] -= 15;
		position += 0.2;
		break;
	case 7:
	case 8:
	case 9:
		anglesg[4] -= 30;
		//anglesg[1] -= 10;
		anglesg[12] -= 10;
		//angles[13] = 0;
		position -= 0.2;
		break;
	case 10:
	case 11:
	case 12:
		anglesg[4] -= 30;
		//anglesg[1] -= 10;
		anglesg[12] -= 10;
		//angles[13] += 15;
		position -= 0.2;
		break;
	}
}
void resetObj(int f){
	for(int i = 0 ; i < PARTSNUM;i++){
		angles[i] = 0.0f;
	}	
	for (int i = 0; i < PARTSNUM; i++) {
		anglesg[i] = 0.0f;
	}
}
void updateObj(int frame){
	gammaok = 0;
	switch (frame) {
	case 0:
		//左手
		angles[1] = -45;
		//右手

		//腿
		angles[13] = 45;

		break;
	case 1:
	case 2:
	case 3:
		angles[4] += 10;
		angles[12] -= 15;
		position += 0.1;
		break;
	case 4:
	case 5:
	case 6:
		angles[4] -= 10;
		angles[12] += 15;
		angles[13] -= 15;
		position -= 0.1;
		break;
	case 7:
	case 8:
	case 9:
		angles[4] -= 10;
		angles[12] += 15;
		angles[13] = 0;
		position += 0.1;
		break;
	case 10:
	case 11:
	case 12:
		angles[4] += 10;
		angles[12] -= 15;
		angles[13] += 15;
		position -= 0.1;
		break;
	}
}


 GLuint M_KaID;
 GLuint M_KdID;
 GLuint M_KsID;

void init(){
	isFrame = false;
	pNo = 0;
	for(int i = 0;i<PARTSNUM;i++)
		angles[i] = 0.0;

	//VAO
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "DSPhong_Material.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER, "DSPhong_Material.fp" },//fragment shader
		{ GL_NONE, NULL }};
	program = LoadShaders(shaders);

	glUseProgram(program);
	
	MatricesIdx = glGetUniformBlockIndex(program,"MatVP");
	ModelID =  glGetUniformLocation(program,"Model");
    M_KaID = glGetUniformLocation(program,"Material.Ka");
	M_KdID = glGetUniformLocation(program,"Material.Kd");
	M_KsID = glGetUniformLocation(program,"Material.Ks");
	//or
	M_KdID = M_KaID+1;
	M_KsID = M_KaID+2;

	Projection = glm::perspective(80.0f,4.0f/3.0f,0.1f,100.0f);
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
	
	// Camera matrix
	View       = glm::lookAt(
		glm::vec3(0,10,25) , 
		glm::vec3(0,0,0), 
		glm::vec3(0,1,0)  
		);

	Obj2Buffer();

	//UBO
	glGenBuffers(1,&UBO);
	glBindBuffer(GL_UNIFORM_BUFFER,UBO);
	glBufferData(GL_UNIFORM_BUFFER,sizeof(mat4)*2,NULL,GL_DYNAMIC_DRAW);
	//get uniform struct size
	int UBOsize = 0;
	glGetActiveUniformBlockiv(program, MatricesIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &UBOsize);  
	//bind UBO to its idx
	glBindBufferRange(GL_UNIFORM_BUFFER,0,UBO,0,UBOsize);
	glUniformBlockBinding(program, MatricesIdx,0);


	glClearColor(0.0,0.0,0.0,1);//black screen
}

#define DOR(angle) (angle*3.1415/180);
void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAO);
	glUseProgram(program);
	loc = glGetUniformLocation(program, "mode");
	glUniform1f(loc, mode);
	float eyey = DOR(eyeAngley);
	View       = lookAt(
		               vec3(eyedistance*sin(degree),5,eyedistance*cos(degree)) , 
		               vec3(0,0,0), 
		               vec3(0,1,0) 
		                );
	updateModels();

	glBindBuffer(GL_UNIFORM_BUFFER,UBO);
	glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(mat4),&View);
	glBufferSubData(GL_UNIFORM_BUFFER,sizeof(mat4),sizeof(mat4),&Projection);
	glBindBuffer(GL_UNIFORM_BUFFER,0);

	GLuint offset[3] = {0,0,0};
	for(int i = 0;i < PARTSNUM ;i++){
		glUniformMatrix4fv(ModelID,1,GL_FALSE,&Models[i][0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,				//location
							  3,				//vec3
							  GL_FLOAT,			//type
							  GL_FALSE,			//not normalized
							  0,				//strip
							  (void*)offset[0]);//buffer offset

		offset[0] +=  vertices_size[i]*sizeof(vec3);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);//location 1 :vec2 UV
		glBindBuffer(GL_ARRAY_BUFFER, uVBO);
		glVertexAttribPointer(1, 
							  2, 
							  GL_FLOAT, 
							  GL_FALSE, 
							  0,
							  (void*)offset[1]);

		offset[1] +=  uvs_size[i]*sizeof(vec2);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);//location 2 :vec3 Normal
		glBindBuffer(GL_ARRAY_BUFFER, nVBO);
		glVertexAttribPointer(2,
							  3, 
							  GL_FLOAT, 
							  GL_FALSE, 
							  0,
							  (void*)offset[2]);

		offset[2] +=  normals_size[i]*sizeof(vec3);

		int vertexIDoffset = 0;
		string mtlname;
		vec3 Ks = vec3(1,1,1);
		for(int j = 0;j <mtls[i].size() ;j++){
			mtlname = mtls[i][j];	

			glUniform3fv(M_KdID,1,&KDs[mtlname][0]);
			glUniform3fv(M_KsID,1,&Ks[0]);

			glDrawArrays(GL_TRIANGLES, vertexIDoffset  , faces[i][j+1]*3);

			vertexIDoffset += faces[i][j+1]*3;
		}
		
	}
	glFlush();
	glutSwapBuffers();
}

void Obj2Buffer(){
	std::vector<vec3> Kds;
	std::vector<vec3> Kas;
	std::vector<vec3> Kss;
	std::vector<std::string> Materials;//mtl-name
	std::string texture;
	//loadMTL("Obj/head2.obj.mtl", Kds, Kas, Kss, Materials, texture);
	loadMTL("Obj/all.obj.mtl", Kds, Kas, Kss, Materials, texture, mode);

	//printf("%d\n",texture);
	for(int i = 0;i<Materials.size();i++){
		string mtlname = Materials[i];

		KDs[mtlname] = Kds[i];
	}

	
	load2Buffer("Obj/Body.obj", 0);

	load2Buffer("Obj/LeftHand.obj", 1);
	load2Buffer("Obj/ball.obj", 2);
	load2Buffer("Obj/ball.obj", 3);
	load2Buffer("Obj/LeftShoulder.obj", 4);

	load2Buffer("Obj/head2.obj", 5);

	load2Buffer("Obj/RightHand.obj", 6);
	load2Buffer("Obj/ball.obj", 7);
	load2Buffer("Obj/ball.obj", 8);
	load2Buffer("Obj/RightShoulder.obj", 9);

	load2Buffer("Obj/ball.obj", 11);
	load2Buffer("Obj/ball.obj", 10);

	load2Buffer("Obj/LeftLeg.obj", 12);
	load2Buffer("Obj/LeftFeet.obj", 13);
	load2Buffer("Obj/ball.obj", 14);

	load2Buffer("Obj/RightLeg.obj", 15);
	load2Buffer("Obj/RightFeet.obj", 16);
	load2Buffer("Obj/ball.obj", 17);
	
	GLuint totalSize[3] = {0,0,0};
	GLuint offset[3] = {0,0,0};
	for(int i = 0;i < PARTSNUM ;i++){
		totalSize[0] += vertices_size[i]*sizeof(vec3);
		totalSize[1] += uvs_size[i] * sizeof(vec2);
		totalSize[2] += normals_size[i] * sizeof(vec3);
	}
	//generate vbo
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&uVBO);
	glGenBuffers(1,&nVBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,totalSize[0],NULL,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uVBO);
	glBufferData(GL_ARRAY_BUFFER,totalSize[1],NULL,GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, nVBO);
	glBufferData(GL_ARRAY_BUFFER,totalSize[2],NULL,GL_STATIC_DRAW);
	
	
	for(int i = 0;i < PARTSNUM ;i++){
		glBindBuffer(GL_COPY_WRITE_BUFFER,VBO);
		glBindBuffer(GL_COPY_READ_BUFFER, VBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[0],vertices_size[i]*sizeof(vec3));
		offset[0] += vertices_size[i]*sizeof(vec3);
		glInvalidateBufferData(VBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);

		glBindBuffer(GL_COPY_WRITE_BUFFER,uVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, uVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[1],uvs_size[i]*sizeof(vec2));
		offset[1] += uvs_size[i]*sizeof(vec2);
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);

		glBindBuffer(GL_COPY_WRITE_BUFFER,nVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, nVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,offset[2],normals_size[i]*sizeof(vec3));
		offset[2] += normals_size[i] * sizeof(vec3);
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER,0);
	}
	glBindBuffer(GL_COPY_WRITE_BUFFER,0);


}

void updateModels(){
	mat4 Rotatation[PARTSNUM];
	mat4 Translation[PARTSNUM];
	for (int i = 0; i < PARTSNUM; i++) {
		Models[i] = mat4(1.0f);
		Rotatation[i] = mat4(1.0f);
		Translation[i] = mat4(1.0f);
	}
	float r, pitch, yaw, roll;
	float alpha, beta, gamma;

	//Body
	beta = angle;
	Rotatation[0] = rotate(beta, 0, 1, 0);
	Translation[0] = translate(0, position, 0);
	Models[0] = Translation[0] * Rotatation[0];
	//左手=======================================================
	//左上手臂



	yaw = DOR(beta); r = 3.7;
	alpha = angles[4];
	gamma = 10;
	float g = gamma + anglesg[4];
	Rotatation[4] = rotate(alpha, 1, 0, 0)*rotate(g, 0, 0, 1);//向前旋轉*向右旋轉
	Translation[4] = translate(5.4, 3.4, 0.5);//位移到左上手臂處
	Models[4] = Models[0] * Translation[4] * Rotatation[4];

	//Rotatation[1] = rotate(alpha, 1, 0, 0)*rotate(gamma, 0, 0, 1);//向前旋轉*向右旋轉
	//Translation[1] = translate(34.5, 26, 1.75);

	//Models[1] = Models[0] * Translation[1] * Rotatation[1];

	pitch = DOR(alpha); r = -5;
	
	alpha = angles[1] + angles[4];
	g = anglesg[1] + anglesg[4] +gamma;
	roll = DOR(g);
	//cout << angles[1] << endl;
	Translation[1] = translate(-r * sin(roll), r*cos(pitch)*cos(roll), r*sin(pitch))*Translation[4];
	Rotatation[1] = rotate(alpha, 1, 0, 0)*rotate(g,0,0,1);
	Models[1] = Translation[1] * Rotatation[1] * Models[1];


	//左肩膀
	

	//左下手臂
	pitch = DOR(alpha); r = 3;
	roll = DOR(gamma);
	static int i = 0;
	i += 5;
	alpha = angles[2] - 20;
	//上手臂+下手臂向前旋轉*向右旋轉
	Rotatation[2] = rotate(alpha, 1, 0, 0);
	//延x軸位移以上手臂為半徑的圓周長:translate(0,r*cos,r*sin)
	//延z軸位移以上手臂為半徑角度:translate(r*sin,-rcos,0)
	Translation[2] = translate(2000, 27.75, -1.5);

	Models[2] = Models[1] * Translation[2] * Rotatation[2];


	pitch = DOR(alpha);
	//b = DOR(angles[2]);
	roll = DOR(gamma);
	//手掌角度與下手臂相同
	//Rotatation[3] = Rotatation[2];
	//延x軸位移以上手臂為半徑的圓周長:translate(0,r*cos,r*sin) ,角度為上手臂+下手臂
	Translation[3] = translate(0, 0, 10);
	Models[3] = Models[2] * Translation[3] * Rotatation[3];
	//============================================================
	//頭==========================================================
	Translation[5] = translate(0,4, -0.5);
	Models[5] = Models[0] * Translation[5] * Rotatation[5];
	//============================================================
	//右手=========================================================
	gamma = -10; alpha = angles[9] = -angles[4];
	anglesg[9] = -anglesg[4];
	g = gamma + anglesg[9];
	Rotatation[9] = rotate(alpha, 1, 0, 0)*rotate(g, 0, 0, 1);
	Translation[9] = translate(-5.1, 3.5, 0.5);
	Models[9] = Models[0] * Translation[9] * Rotatation[9];


	angles[6] = angles[1];
	anglesg[6] = -anglesg[1];
	pitch = DOR(alpha); r = -5;
	
	alpha = angles[6] + angles[9];
	g = gamma+ anglesg[6] + anglesg[9];
	roll = DOR(g);
	Translation[6] = translate(-r * sin(roll), r*cos(pitch)*cos(roll), r*sin(pitch))*Translation[9];
	Rotatation[6] = rotate(alpha, 1, 0, 0)*rotate(g,0,0,1);
	Models[6] = Translation[6] * Rotatation[6] * Models[6];

	angles[7] = angles[2];
	pitch = DOR(alpha); r = -3;
	roll = DOR(gamma);
	alpha = angles[7] - 20;
	Rotatation[7] = rotate(alpha, 1, 0, 0);
	Translation[7] = translate(0, -500, 0);
	Models[7] = Models[6] * Translation[7] * Rotatation[7];

	pitch = DOR(alpha);
	//b = DOR(angles[7]);
	roll = DOR(gamma);
	Translation[8] = translate(0, 0, 2000);
	Models[8] = Models[7] * Translation[8] * Rotatation[8];
	//=============================================================
	//back&DBody===================================================
	Translation[10] = translate(0, 2000, -4.5);
	Models[10] = Models[0] * Translation[10] * Rotatation[10];

	Translation[11] = translate(0, -5300, 0);
	Models[11] = Models[0] * Translation[11] * Rotatation[11];
	//=============================================================
	//左腳
	alpha = angles[12]; gamma = 10;
	g = gamma + anglesg[12];
	Rotatation[12] = rotate(alpha, 1, 0, 0)*rotate(g, 0, 0, 1);
	Translation[12] = translate(2.25, -4, 1);
	Models[12] = Translation[12] * Rotatation[12] * Models[12];

	pitch = DOR(alpha); r = -7.5;
	
	alpha = angles[13] + angles[12];
	g = gamma+ anglesg[13] + anglesg[12];
	roll = DOR(g);
	Translation[13] = translate(-r * sin(roll), r*cos(pitch)*cos(roll), r*sin(pitch))*Translation[12];
	Rotatation[13] = rotate(alpha, 1, 0, 0)*rotate(g, 0, 0, 1);
	Models[13] = Translation[13] * Rotatation[13] * Models[13];

	pitch = DOR(alpha); r = -5;
	//b = DOR(angles[13]);
	roll = DOR(gamma);
	Translation[14] = translate(-2000, -2, 1)*Translation[12];
	Rotatation[14] = rotate(alpha, 1, 0, 0)*rotate(gamma, 0, 0, 1);
	Models[14] = Translation[14] * Rotatation[14] * Models[14];
	//=============================================================
	//右腳
	alpha = angles[15] = -angles[12];
	gamma = -10;
	anglesg[15] = -anglesg[12];
	g = gamma + anglesg[15];
	Rotatation[15] = rotate(alpha, 1, 0, 0)*rotate(g, 0, 0, 1);
	Translation[15] = translate(-2.25, -4.1, 1);
	Models[15] = Translation[15] * Rotatation[15] * Models[15];

	angles[16] = angles[13];
	anglesg[16] = -anglesg[13];
	g = gamma + anglesg[16]+anglesg[15];
	pitch = DOR(alpha); r = -7.5;
	roll = DOR(g);
	alpha = angles[16] + angles[15];
	Rotatation[16] = rotate(alpha, 1, 0, 0)*rotate(g, 0, 0, 1);
	Translation[16] = translate(-r * sin(roll), r*cos(pitch)*cos(roll), r*sin(pitch))*Translation[15];
	Models[16] = Translation[16] * Rotatation[16] * Models[16];

	pitch = DOR(alpha); r = -5;
	//b = DOR(angles[16]);
	roll = DOR(gamma);
	alpha = angles[15] + angles[16];
	Translation[17] = translate(-0.75, -8000, 0)*Translation[15];
	Rotatation[17] = rotate(alpha, 1, 0, 0);
	Models[17] = Translation[17] * Rotatation[17] * Models[17];
	//=============================================================
}

void load2Buffer(char* obj,int i){
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals; // Won't be used at the moment.
	std::vector<unsigned int> materialIndices;

	bool res = loadOBJ(obj, vertices, uvs, normals,faces[i],mtls[i]);
	if(!res) printf("load failed\n");

	//glUseProgram(program);

	glGenBuffers(1,&VBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
	glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(vec3),&vertices[0],GL_STATIC_DRAW);
	vertices_size[i] = vertices.size();

	//(buffer type,data起始位置,data size,data first ptr)
	//vertices_size[i] = glm_model->numtriangles;
	
	//printf("vertices:%d\n",vertices_size[);

	glGenBuffers(1,&uVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, uVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);
	uvs_size[i] = uvs.size();

	glGenBuffers(1,&nVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, nVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
	normals_size[i] = normals.size();
}
mat4 translate(float x,float y,float z){
	vec4 t = vec4(x,y,z,1);//w = 1 ,則x,y,z=0時也能translate
	vec4 c1 = vec4(1,0,0,0);
	vec4 c2 = vec4(0,1,0,0);
	vec4 c3 = vec4(0,0,1,0);
	mat4 M = mat4(c1,c2,c3,t);
	return M;
} 
mat4 scale(float x,float y,float z){
	vec4 c1 = vec4(x,0,0,0);
	vec4 c2 = vec4(0,y,0,0);
	vec4 c3 = vec4(0,0,z,0);
	vec4 c4 = vec4(0,0,0,1);
	mat4 M = mat4(c1,c2,c3,c4);
	return M;
}

mat4 rotate(float angle,float x,float y,float z){
	float r = DOR(angle);
	mat4 M = mat4(1);

	vec4 c1 = vec4(cos(r)+(1-cos(r))*x*x,(1-cos(r))*y*x+sin(r)*z,(1-cos(r))*z*x-sin(r)*y,0);
	vec4 c2 = vec4((1-cos(r))*y*x-sin(r)*z,cos(r)+(1-cos(r))*y*y,(1-cos(r))*z*y+sin(r)*x,0);
	vec4 c3 = vec4((1-cos(r))*z*x+sin(r)*y,(1-cos(r))*z*y-sin(r)*x,cos(r)+(1-cos(r))*z*z,0);
	vec4 c4 = vec4(0,0,0,1);
	M = mat4(c1,c2,c3,c4);
	return M;
}
void Keyboard(unsigned char key, int x, int y){
	switch(key){
	case '1':
		angle += 5;
		if(angle>=360) angle = 0;
		printf("beta:%f\n",angle);
		break;
	case '2':
		angle -= 5;
		if(angle<=0) angle = 360;
		printf("beta:%f\n",angle);
		break;
	case 'w':
		eyedistance -= 0.2;
		break;
	case 's':
		eyedistance += 0.2;
		break;
	case 'a':
		eyeAngley -=10;
		break;
	case 'd':
		eyeAngley +=10;
		break;
	case 'r':
		angles[1] -= 5; 
		if(angles[1] == -360) angles[1] = 0;
		movey = 0;
		movex = 0;
		break;
	case 't':
		angles[2] -= 5;
		if(angles[2] == -360) angles[2] = 0;
		movey = 0;
		movex = 0;
		break;
	case 'q':
		break;
	case 'e':
		break;
	}
	glutPostRedisplay();
}
void menuEvents(int option){}
void ActionMenuEvents(int option){
	switch(option){
	case 0:
		action = IDLE;
		break;
	case 1:
		action = WALK;
		break;
	case 2:
		action = DANCE;
		break;
	}
}
void ModeMenuEvents(int option){
	switch(option){
	case 0:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 2:
		loc = glGetUniformLocation(program, "mode");
		glUniform1i(loc, option);
		break;
	case 3:
		loc = glGetUniformLocation(program, "mode");
		glUniform1i(loc, option);
		break;
	}
}
void ShaderMenuEvents(int option){
	pNo = option;
}

