// The sample robotarm model.  You should build a file
// very similar to this for when you make your model.
#pragma warning (disable : 4305)
#pragma warning (disable : 4244)
#pragma warning(disable : 4786)
#pragma warning (disable : 4312)


#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "particleSystem.h"



#include "mat.h"
#include <FL/gl.h>
#include <cstdlib>

using namespace std;

#define M_DEFAULT 2.0f
#define M_OFFSET 3.0f
#define P_OFFSET 0.3f
#define MAX_VEL 200
#define MIN_STEP 0.1





// This is a list of the controls for the Transformer
// We'll use these constants to access the values 
// of the controls from the user interface.
enum TransformerControls
{ 
    BASE_ROTATION=0, TIRE_ROTATION, LEFT_ARM_ROTATION, RIGHT_ARM_ROTATION, LOWER_TILT, UPPER_TILT, CLAW_ROTATION,
        EXTEND_ARMS, LOWER_LENGTH, UPPER_LENGTH, PARTICLE_COUNT, NUMCONTROLS, 
};

void ground(float h);
void base(float h);
void trunk();
void body(float arms_position);
void head(float h);
void arm(float position, float rotation);
void lower_arm(float h);
void upper_arm(float h);
void claw(float h);
void y_box(float h);
Mat4f glGetMatrix(GLenum pname);
Vec3f getWorldPoint(Mat4f matCamXforms);

// To make a Transformer, we inherit off of ModelerView
class Transformer : public ModelerView 
{
public:
    Transformer(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) {}
    virtual void draw();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createTransformer(int x, int y, int w, int h, char *label)
{ 
    return new Transformer(x,y,w,h,label); 
}

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))


// Utility function.  Use glGetMatrix(GL_MODELVIEW_MATRIX) to retrieve
//  the current ModelView matrix.
Mat4f glGetMatrix(GLenum pname)
{
    GLfloat m[16];
    glGetFloatv(pname, m);
    Mat4f matCam(m[0],  m[1],  m[2],  m[3],
                            m[4],  m[5],  m[6],  m[7],
                            m[8],  m[9],  m[10], m[11],
                            m[12], m[13], m[14], m[15] );

    // because the matrix GL returns is column major...
    return matCam.transpose();
}





// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out Transformer
void Transformer::draw()
{
	/* pick up the slider values */

    float theta = VAL( BASE_ROTATION );
	float theta_tires = -5.0 * VAL( TIRE_ROTATION );
	float phi = VAL( LOWER_TILT );
    float phi_l = VAL ( LEFT_ARM_ROTATION );
    float phi_r = VAL ( RIGHT_ARM_ROTATION );
	float psi = VAL( UPPER_TILT );
	float cr = VAL( CLAW_ROTATION );
	float h1 = VAL( EXTEND_ARMS );
	float h2 = VAL( LOWER_LENGTH );
	float h3 = VAL( UPPER_LENGTH );
	float pc = VAL( PARTICLE_COUNT );

    // This call takes care of a lot of the nasty projection 
    // matrix stuff
    ModelerView::draw();

    // Save the camera transform that was applied by
    // ModelerView::draw() above.
    // While we're at it, save an inverted copy of this matrix.  We'll
    // need it later.
    Mat4f matCam = glGetMatrix( GL_MODELVIEW_MATRIX );
    //Mat4f matCamInverse = matCam.inverse();



	static GLfloat lmodel_ambient[] = {0.4,0.4,0.4,1.0};

	// define the model

	ground(-0.2);

    // center rod
    setDiffuseColor( 1.0, 0.0, 0.0 );
    setAmbientColor( 1.0, 0.0, 0.0 );
    glPushMatrix();
        glRotatef(-90.0, 1.0, 0.0, 0.0);
        drawCylinder(5.0, 0.1, 0.1);
    glPopMatrix();

    glRotatef( theta, 0.0, 1.0, 0.0 );		// turn the whole assembly around the y-axis. 
    base(theta_tires);

    glTranslatef( 0.0, 0.75, 0.0 );         // move to the top of the base
    trunk();

    glTranslatef( -1.0, 0.75, 0.0 );         // set rotation axis
    glRotatef(phi, 0.0, 0.0, 1.0);
    
    glTranslatef(1.0, -0.75, 0.0);
    body(h1);                              // draw the body
    // head(0.0);

 //    glTranslatef( 0.0, h1, 0.0 );			// move to the top of the base
	// glPushMatrix();
	// 		glTranslatef( 0.5, h1, 0.6 );	
	// glPopMatrix();
 //    glRotatef( phi, 0.0, 0.0, 1.0 );		// rotate around the z-axis for the lower arm
	// glTranslatef( -0.1, 0.0, 0.4 );
	// lower_arm(h2);							// draw the lower arm

 //    glTranslatef( 0.0, h2, 0.0 );			// move to the top of the lower arm
 //    glRotatef( psi, 0.0, 0.0, 1.0 );		// rotate  around z-axis for the upper arm
	// upper_arm(h3);							// draw the upper arm

	// glTranslatef( 0.0, h3, 0.0 );
	// glRotatef( cr, 0.0, 0.0, 1.0 );
	// claw(1.0);




	//*** DON'T FORGET TO PUT THIS IN YOUR OWN CODE **/
	endDraw();
}

void ground(float h) 
{
	glDisable(GL_LIGHTING);
	glColor3f(0.65,0.45,0.2);
	glPushMatrix();
	glScalef(50,0,50);
	y_box(h);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void base(float theta) {
	setDiffuseColor( 0.25, 0.25, 0.25 );
	setAmbientColor( 0.25, 0.25, 0.25 );
    glPushMatrix();
        // trunk
        // top left
        glPushMatrix();
            glTranslatef(1.5, 0.5, 1.0);
            glRotatef( theta, 0.0, 0.0, 1.0 );
            drawCylinder(0.25, 0.5, 0.5);
        glPopMatrix();
        // bottom left
        glPushMatrix();
            glTranslatef(1.5, 0.5, -1.25);
            glRotatef( theta, 0.0, 0.0, 1.0 );
            drawCylinder(0.25, 0.5, 0.5);
        glPopMatrix();
        // top right
        glPushMatrix();
            glTranslatef(-1.5, 0.5, 1.0);
            glRotatef( theta, 0.0, 0.0, 1.0 );
            drawCylinder(0.25, 0.5, 0.5);
        glPopMatrix();
        // bottom right
        glPushMatrix();
            glTranslatef(-1.5, 0.5, -1.25);
            glRotatef( theta, 0.0, 0.0, 1.0 );
            drawCylinder(0.25, 0.5, 0.5);
        glPopMatrix();
    glTranslatef(-2, 0.5, -1.0);
    drawBox(4.0,0.25,2.0);
    glPopMatrix();
}

void trunk(){
    setDiffuseColor( 0.1, 0.0, 1.0 );
    setAmbientColor( 0.1, 0.0, 1.0 );
    glPushMatrix();
        glTranslatef(-2.0, 0.0, -1.0);
        drawBox(1.0,0.75,2.0);
    glPopMatrix();
}

void body(float arms_position) {
    glPushMatrix();
        // head
        setDiffuseColor( 0.5, 0.5, 0.5 );
        setAmbientColor( 0.5, 0.5, 0.5 );
        glPushMatrix();
            // glRotatef(45.0, 1.0, 0.0, 0.0);
            glTranslatef(1.0, 0.0, -0.25);
            drawBox(0.5,0.8,0.5);
        glPopMatrix();
        // torso
        setDiffuseColor( 0.4, 0.4, 0.4 );
        setAmbientColor( 0.4, 0.4, 0.4 );
        glPushMatrix();
            glTranslatef(-1.0, 0.0, -0.5);
            drawBox(2.0,0.75,1.0);
        glPopMatrix();
        setDiffuseColor( 0.1, 0.0, 1.0 );
        setAmbientColor( 0.1, 0.0, 1.0 );
        // left arm
        arm(-(arms_position + 1.0), 0.0);
        // right arm
        arm(arms_position + 0.5, 0.0);
        // cab
        glPushMatrix();
            glTranslatef(-1.0, 0.75, -1.0);
            drawBox(2.0,0.75,2.0);
        glPopMatrix();
        // front
        glPushMatrix();
            glTranslatef(1.0, 0.0, -1.0);
            drawBox(1.0,0.75,2.0);
        glPopMatrix();
        // extra stuff
        glPushMatrix();
            glTranslatef(-1.75, 0.0, -1.0);
            drawBox(0.75,0.75,2.0);
        glPopMatrix();
        glPushMatrix();
            setDiffuseColor( 0.25, 0.25, 0.25 );
            setAmbientColor( 0.25, 0.25, 0.25 );
            glTranslatef(0.75, 0.375, -(arms_position + 0.5) );
            drawCylinder((arms_position + 0.5)*2.0,0.25,0.25);
        glPopMatrix();
        
    glPopMatrix();
}

void head( float h ){
    setDiffuseColor( 0.5, 0.5, 0.5 );
    setAmbientColor( 0.5, 0.5, 0.5 );
    glPushMatrix();
        // glRotatef(45.0, 1.0, 0.0, 0.0);
        glTranslatef(1.0, 0.0, 0.0);
        drawBox(0.5,0.6,0.5);
    glPopMatrix();
}

void arm( float position, float rotation){
    glPushMatrix();
        glTranslatef(-1.0, 0.0, position );
        drawBox(2.0,0.75,0.5);
    glPopMatrix();
}

void lower_arm(float h) {					// draw the lower arm
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );
	y_box(h);
}

void upper_arm(float h) {					// draw the upper arm
	setDiffuseColor( 0.85, 0.75, 0.25 );
	setAmbientColor( 0.95, 0.75, 0.25 );
	glPushMatrix();
	glScalef( 1.0, 1.0, 0.7 );
	y_box(h);
	glPopMatrix();
}

void claw(float h) {
	setDiffuseColor( 0.25, 0.25, 0.85 );
	setAmbientColor( 0.25, 0.25, 0.85 );

	glBegin( GL_TRIANGLES );

	glNormal3d( 0.0, 0.0, 1.0);		// +z side
	glVertex3d( 0.5, 0.0, 0.5);
	glVertex3d(-0.5, 0.0, 0.5);
	glVertex3d( 0.5,   h, 0.5);

	glNormal3d( 0.0, 0.0, -1.0);	// -z side
	glVertex3d( 0.5, 0.0, -0.5);
	glVertex3d(-0.5, 0.0, -0.5);
	glVertex3d( 0.5,   h, -0.5);

	glEnd();

	glBegin( GL_QUADS );

	glNormal3d( 1.0,  0.0,  0.0);	// +x side
	glVertex3d( 0.5, 0.0,-0.5);
	glVertex3d( 0.5, 0.0, 0.5);
	glVertex3d( 0.5,   h, 0.5);
	glVertex3d( 0.5,   h,-0.5);

	glNormal3d( 0.0,-1.0, 0.0);		// -y side
	glVertex3d( 0.5, 0.0, 0.5);
	glVertex3d( 0.5, 0.0,-0.5);
	glVertex3d(-0.5, 0.0,-0.5);
	glVertex3d(-0.5, 0.0, 0.5);

	glEnd();
}

void y_box(float h) {

	glBegin( GL_QUADS );

	glNormal3d( 1.0 ,0.0, 0.0);			// +x side
	glVertex3d( 0.25,0.0, 0.25);
	glVertex3d( 0.25,0.0,-0.25);
	glVertex3d( 0.25,  h,-0.25);
	glVertex3d( 0.25,  h, 0.25);

	glNormal3d( 0.0 ,0.0, -1.0);		// -z side
	glVertex3d( 0.25,0.0,-0.25);
	glVertex3d(-0.25,0.0,-0.25);
	glVertex3d(-0.25,  h,-0.25);
	glVertex3d( 0.25,  h,-0.25);

	glNormal3d(-1.0, 0.0, 0.0);			// -x side
	glVertex3d(-0.25,0.0,-0.25);
	glVertex3d(-0.25,0.0, 0.25);
	glVertex3d(-0.25,  h, 0.25);
	glVertex3d(-0.25,  h,-0.25);

	glNormal3d( 0.0, 0.0, 1.0);			// +z side
	glVertex3d(-0.25,0.0, 0.25);
	glVertex3d( 0.25,0.0, 0.25);
	glVertex3d( 0.25,  h, 0.25);
	glVertex3d(-0.25,  h, 0.25);

	glNormal3d( 0.0, 1.0, 0.0);			// top (+y)
	glVertex3d( 0.25,  h, 0.25);
	glVertex3d( 0.25,  h,-0.25);
	glVertex3d(-0.25,  h,-0.25);
	glVertex3d(-0.25,  h, 0.25);

	glNormal3d( 0.0,-1.0, 0.0);			// bottom (-y)
	glVertex3d( 0.25,0.0, 0.25);
	glVertex3d(-0.25,0.0, 0.25);
	glVertex3d(-0.25,0.0,-0.25);
	glVertex3d( 0.25,0.0,-0.25);

	glEnd();
}

int main()
{
    ModelerControl controls[NUMCONTROLS ];

    controls[BASE_ROTATION] = ModelerControl("base rotation (theta)", -180.0, 180.0, 0.1, 0.0 );
	controls[TIRE_ROTATION] = ModelerControl("tire rotation (tires_theta)", 0.0, 360.0, 0.1, 0.0 );
    controls[LOWER_TILT] = ModelerControl("main torso (phi)", 0.0, 90.0, 0.1, 0.0 );
    controls[UPPER_TILT] = ModelerControl("upper arm tilt (psi)", 0.0, 135.0, 0.1, 30.0 );
    controls[CLAW_ROTATION] = ModelerControl("claw rotation (cr)", -30.0, 180.0, 0.1, 0.0 );
    controls[EXTEND_ARMS] = ModelerControl("extend arms (h1)", 0.0, 0.7, 0.05, 0.0 );
    controls[LEFT_ARM_ROTATION] = ModelerControl("left arm rotation (phi_l)", -180.0, 180.0, 0.1, 0.0 );
    controls[RIGHT_ARM_ROTATION] = ModelerControl("right arm rotation (phi_r)", -180.0, 180.0, 0.1, 0.0 );
    controls[LOWER_LENGTH] = ModelerControl("lower arm length (h2)", 1, 10.0, 0.1, 3.0 );
    controls[UPPER_LENGTH] = ModelerControl("upper arm length (h3)", 1, 10.0, 0.1, 2.5 );
    controls[PARTICLE_COUNT] = ModelerControl("particle count (pc)", 0.0, 5.0, 0.1, 5.0 );
    

	// You should create a ParticleSystem object ps here and then
	// call ModelerApplication::Instance()->SetParticleSystem(ps)
	// to hook it up to the animator interface.

    ModelerApplication::Instance()->Init(&createTransformer, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}
