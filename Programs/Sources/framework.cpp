
//=============================================================================================
// Collection of classes from lecture slides.
// Framework for assignments. Valid from 2019.
// Do not change it if you want to submit a homework.
//=============================================================================================

#include "framework.h"
#include "Assets.h"
#include "GlutCallbacks.h"

// Initialization
void onInitialization();

// Window has become invalid: Redraw
void onDisplay();

// Key of ASCII code pressed
void onKeyboard(unsigned char key, int pX, int pY);

// Key of ASCII code released
void onKeyboardUp(unsigned char key, int pX, int pY);

// Move mouse with key pressed
void onMouseMotion(int pX, int pY);

// Mouse click event
void onMouse(int button, int state, int pX, int pY);

// Idle event indicating that some time elapsed: do animation here
void onIdle();

void cleanUp() {
	delete scene;
	Assets::cleanUp();

	for (IControlEvent* event : controlEvents) {
		delete event;
	}
}

// Entry point of the application
int main(int argc, char * argv[]) {

	// Initialize GLUT, Glew and OpenGL 
	glutInit(&argc, argv);

	// OpenGL major and minor versions
	int majorVersion = 3, minorVersion = 3;
#if !defined(__APPLE__)
	glutInitContextVersion(majorVersion, minorVersion);
#endif
	glutInitWindowSize(windowWidth, windowHeight);				// Application window is initially of resolution 600x600
	glutInitWindowPosition(100, 100);							// Relative location of the application window
#if defined(__APPLE__)
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);  // 8 bit R,G,B,A + double buffer + depth buffer
#else
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
	glutCreateWindow(argv[0]);
	if (fullScreenMode) {
		glutFullScreen();							// Full screen
	}

#if !defined(__APPLE__)
	glewExperimental = true;	// magic
	glewInit();
#endif
	printf("GL Vendor    : %s\n", glGetString(GL_VENDOR));
	printf("GL Renderer  : %s\n", glGetString(GL_RENDERER));
	printf("GL Version (string)  : %s\n", glGetString(GL_VERSION));
	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
	printf("GL Version (integer) : %d.%d\n", majorVersion, minorVersion);
	printf("GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Initialize this program and getInstance shaders
	onInitialization();
	glutDisplayFunc(onDisplay);                // Register event handlers
	glutMouseFunc(onMouse);
	glutIdleFunc(onIdle);
	glutKeyboardFunc(onKeyboard);
	glutKeyboardUpFunc(onKeyboardUp);
	glutMotionFunc(onMouseMotion);
	glutPassiveMotionFunc(onMouseMotion);
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	glutSetWindowTitle(WINDOW_TITLE);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	glutMainLoop();

	cleanUp();
	return 0;
}

void GPUProgram::getErrorInfo(unsigned int handle) { // shader error report
	int logLen, written;
	glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLen);
	if (logLen > 0) {
		std::string log(logLen, '\0');
		glGetShaderInfoLog(handle, logLen, &written, &log[0]);
		printf("Shader log:\n%s", log.c_str());
		if (waitError) getchar();
	}
}

bool GPUProgram::checkShader(unsigned int shader, std::string message) { // check if shader could be compiled
	int OK;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &OK);
	if (!OK) {
		printf("%s!\n", message.c_str());
		getErrorInfo(shader);
		return false;
	}
	return true;
}

bool GPUProgram::checkLinking(unsigned int program) { 	// check if shader could be linked
	int OK;
	glGetProgramiv(program, GL_LINK_STATUS, &OK);
	if (!OK) {
		printf("Failed to link shader program!\n");
		getErrorInfo(program);
		return false;
	}
	return true;
}

// Get the address of a GPU uniform variable

int GPUProgram::getLocation(const std::string& name) {
	int location = glGetUniformLocation(shaderProgramId, name.c_str());
	if (location < 0) printf("uniform %s cannot be set\n", name.c_str());
	return location;
}

GPUProgram::GPUProgram(const GPUProgram& program) {
	if (program.shaderProgramId > 0) printf("\nError: GPU program is not copied on GPU!!!\n");
}

bool GPUProgram::create(const char* const vertexShaderSource, const char* const fragmentShaderSource, const char* const fragmentShaderOutputName, const char* const geometryShaderSource)
{
	// Create vertex shader from string
	if (vertexShader == 0) vertexShader = glCreateShader(GL_VERTEX_SHADER);
	if (!vertexShader) {
		printf("Error in vertex shader creation\n");
		exit(1);
	}
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	if (!checkShader(vertexShader, "Vertex shader error")) return false;

	// Create geometry shader from string if given
	if (geometryShaderSource != nullptr) {
		if (geometryShader == 0) geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		if (!geometryShader) {
			printf("Error in geometry shader creation\n");
			exit(1);
		}
		glShaderSource(geometryShader, 1, (const GLchar**)&geometryShaderSource, NULL);
		glCompileShader(geometryShader);
		if (!checkShader(geometryShader, "Geometry shader error")) return false;
	}

	// Create fragment shader from string
	if (fragmentShader == 0) fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fragmentShader) {
		printf("Error in fragment shader creation\n");
		exit(1);
	}

	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	if (!checkShader(fragmentShader, "Fragment shader error")) return false;

	shaderProgramId = glCreateProgram();
	if (!shaderProgramId) {
		printf("Error in shader program creation\n");
		exit(1);
	}
	glAttachShader(shaderProgramId, vertexShader);
	glAttachShader(shaderProgramId, fragmentShader);
	if (geometryShader > 0) glAttachShader(shaderProgramId, geometryShader);

	// Connect the fragmentColor to the frame buffer memory
	glBindFragDataLocation(shaderProgramId, 0, fragmentShaderOutputName);	// this output goes to the frame buffer memory

																			// program packaging
	glLinkProgram(shaderProgramId);
	if (!checkLinking(shaderProgramId)) return false;

	// make this program run
	glUseProgram(shaderProgramId);
	return true;
}
