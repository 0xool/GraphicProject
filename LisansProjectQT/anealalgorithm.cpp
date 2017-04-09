#include "anealalgorithm.h"
#include "qdebug.h"

#define NO 0
#define YES 1
#define COLLINEAR 2

AnealAlgorithm::AnealAlgorithm()
{

    qDebug() << "AA RUNED!!!";

    this->mesh = pm->instance().getMesh();
    this->xMax = pm->instance().getOuterShape().xMax;
    this->yMin = pm->instance().getOuterShape().yMin;

    this->xMin = pm->instance().getOuterShape().xMin;
    this->yMax = pm->instance().getOuterShape().yMax;

}

//=================================================================================================================================
// Initialization for the SA Class.
AnealAlgorithm::AnealAlgorithm(float xMin, float yMin, float xMax, float yMax , Mesh mesh , OuterShape outerShape)
{


    this->mesh = mesh;

}

//=================================================================================================================================
// For given Node this function will implement and create a new graph based on evaluation and cost.
void AnealAlgorithm::SimulatedAnnealingForGraph()
{

    //first randome state given to choose
    Mesh tempMesh;
    qDebug() << pm->instance().getMesh().getMesh().size() << "main Mesh size :D";
    for (int i = 0 ; i < this->mesh.getMesh().size() ; i++)
    {

        Node tempNode = this->mesh.getMesh()[i];

        float randomX = rand() % (int)(this->xMax - this->xMin + 1) ;
        float randomY = (rand() % (int)(this->yMax - this->yMin + 1) );

        tempNode.setX(randomX + this->xMin);
        tempNode.setY(randomY + this->yMin);

        if (pm->instance().getDebug() == true)
        {
            qDebug() << tempNode.getX() << ": tempNode X ";
            qDebug() << tempNode.getY() << ": tempNode Y";
        }

        tempMesh.addNodeToMesh(tempNode);



    }

    this->meshStates.push_back(tempMesh);
    pm->instance().setMesh(tempMesh);
    qDebug() << pm->instance().getMesh().getMesh().size() << "Mesh size :D";

    
    
}
//=================================================================================================================================
// Optional behaviour for the class(i will not probably use this one.).
std::vector<Node> AnealAlgorithm::edgeDetection()
{



}
//=================================================================================================================================


//=================================================================================================================================
// Here we test if the given point do intersect or not(As defined for each output.).
int areIntersecting(
    float v1x1, float v1y1, float v1x2, float v1y2,
    float v2x1, float v2y1, float v2x2, float v2y2)
{
    float d1, d2;
    float a1, a2, b1, b2, c1, c2;

    // Convert vector 1 to a line (line 1) of infinite length.
    // We want the line in linear equation standard form: A*x + B*y + C = 0
    // See: http://en.wikipedia.org/wiki/Linear_equation
    a1 = v1y2 - v1y1;
    b1 = v1x1 - v1x2;
    c1 = (v1x2 * v1y1) - (v1x1 * v1y2);

    // Every point (x,y), that solves the equation above, is on the line,
    // every point that does not solve it, is not. The equation will have a
    // positive result if it is on one side of the line and a negative one
    // if is on the other side of it. We insert (x1,y1) and (x2,y2) of vector
    // 2 into the equation above.
    d1 = (a1 * v2x1) + (b1 * v2y1) + c1;
    d2 = (a1 * v2x2) + (b1 * v2y2) + c1;

    // If d1 and d2 both have the same sign, they are both on the same side
    // of our line 1 and in that case no intersection is possible. Careful,
    // 0 is a special case, that's why we don't test ">=" and "<=",
    // but "<" and ">".
    if (d1 > 0 && d2 > 0) return NO;
    if (d1 < 0 && d2 < 0) return NO;

    // The fact that vector 2 intersected the infinite line 1 above doesn't
    // mean it also intersects the vector 1. Vector 1 is only a subset of that
    // infinite line 1, so it may have intersected that line before the vector
    // started or after it ended. To know for sure, we have to repeat the
    // the same test the other way round. We start by calculating the
    // infinite line 2 in linear equation standard form.
    a2 = v2y2 - v2y1;
    b2 = v2x1 - v2x2;
    c2 = (v2x2 * v2y1) - (v2x1 * v2y2);

    // Calculate d1 and d2 again, this time using points of vector 1.
    d1 = (a2 * v1x1) + (b2 * v1y1) + c2;
    d2 = (a2 * v1x2) + (b2 * v1y2) + c2;

    // Again, if both have the same sign (and neither one is 0),
    // no intersection is possible.
    if (d1 > 0 && d2 > 0) return NO;
    if (d1 < 0 && d2 < 0) return NO;

    // If we get here, only two possibilities are left. Either the two
    // vectors intersect in exactly one point or they are collinear, which
    // means they intersect in any number of points from zero to infinite.
    if ((a1 * b2) - (a2 * b1) == 0.0f) return COLLINEAR;

    // If they are not collinear, they must intersect in exactly one point.
    return YES;
}
