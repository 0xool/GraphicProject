#include "anealalgorithm.h"
#include "qdebug.h"
#include <unistd.h>

#include <thread>

#define NO 0
#define YES 1
#define COLLINEAR 2

#define TEMPRETURE 1000
#define COOLINGRATE 0.003

AnealAlgorithm::AnealAlgorithm()
{

    if (pm->instance().getDebug() == true)
        qDebug() << "AA RUNED!!!";
        //pm->mainAlgorithmView->updateView();

    this->mesh = pm->instance().getMesh();
    this->xMax = pm->instance().getOuterShape().xMax;
    this->yMin = pm->instance().getOuterShape().yMin;

    this->innerShape = pm->instance().getInnserShape();
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
            qDebug() << pm->instance().getMesh().getMeshPointer().size() << "main Mesh size :D";
            for (int i = 0 ; i < this->mesh.getMeshPointer().size() ; i++)
            {

                Node *tempNode = this->mesh.getMeshPointer()[i];

                float randomX = rand() % (int)(this->xMax - this->xMin + 1) ;
                float randomY = rand() % (int)(this->yMax - this->yMin + 1);

                tempNode->setX(randomX + this->xMin);
                tempNode->setY(randomY + this->yMin);

                if (pm->instance().getDebug() == true)
                {
                    qDebug() << tempNode->getX() << ": tempNode X ";
                    qDebug() << tempNode->getY() << ": tempNode Y ";
                }

                tempMesh.addNodePointerToMesh(tempNode);
            }

            for (int i = 0 ; i < tempMesh.getMeshPointer().size() ; i++)
            {
                Node* tempNode = tempMesh.getMeshPointer()[i];

                 qDebug() << "tempMesh size : " << tempMesh.getMeshPointer().size();
                   for (int j = 0 ; j < tempMesh.getMeshPointer().size() ; j++)
                {
                    std::vector<Node*> nodeVector = tempNode->getChildNodes();
                    Node* testNode = tempMesh.getMeshPointer()[j];
            //            qDebug() << tempNode->getY() << "WTF :D.";
                    if(std::find(nodeVector.begin(), nodeVector.end(),  testNode) != nodeVector.end()) {
                        // v contains x
                        if (pm->instance().getDebug() == true)
                        {
                            qDebug() << "contains Element";
                            qDebug("test node : " + testNode->getName().toLatin1());
                            qDebug("temp node : " + tempNode->getName().toLatin1());

                        }

                      int cost = calculateIntersectionCost(innerShape , tempNode , testNode);
                      int currentCost = cost + tempMesh.getCost();

                      tempMesh.setCost( currentCost );


                    } else {
                        // v does not contain x
                        if (pm->instance().getDebug() == true)
                        {
                            qDebug() << "does not contain element";
                        }


                    }
                }
            }

            qDebug() << "THE MESH COST IS : " << tempMesh.getCost();

            if (tempMesh.getCost() == 0)
            {
                if (pm->instance().getDebug() == true)
                    qDebug() << "Found preMature Answer.";
                this->haveAnswer = true;
                pm->instance().setSolutionMesh(tempMesh);

            }

            this->meshStates.push_back(tempMesh);
            //here we calculate the cost per mesh added

             qDebug() << tempMesh.getMeshPointer().size() << "Mesh size :D";
            pm->instance().setMesh(tempMesh);

            if (pm->instance().getDebug() == true)
            {
                qDebug() << pm->instance().getMesh().getMesh().size() << "Mesh size :D";
            }

            pm->instance().mainAlgorithmView->update();


    
}
//=================================================================================================================================
// Optional behaviour for the class(i will not probably use this one.).
std::vector<Node> AnealAlgorithm::edgeDetection()
{



}
//=================================================================================================================================
float AnealAlgorithm::acceptanceRate(int cost, int newCost, float temp)
{

    if (newCost < cost){
        return 1;
    }else {
        return exp((cost - newCost) / temp);
    }

}
//=================================================================================================================================
Mesh AnealAlgorithm::simulatedAnnealingAlgorithm()
{

            // Set initial temp
            double temp = TEMPRETURE;

            // Cooling rate
            double coolingRate = COOLINGRATE;

           // Set as current best
           Mesh currentMesh = pm->instance().getMesh();
           Mesh bestMesh = currentMesh;

           qDebug() << "Algorithm started.";
           // Loop until system has cooled
           while (temp > 1 && !haveAnswer) {

               qDebug() << "TEMPRETURE : " << temp;

               if (bestMesh.getCost() == 0)
                   break;
               // Create new neighbour tour
               Mesh neighborMesh = currentMesh;


               // Get a random positions in the tour
               int currentMeshSize = (int)(currentMesh.getMeshPointer().size()) - 1;
               int randomNodePosition = rand() % currentMeshSize;


               Node *tempNode = neighborMesh.getMeshPointer()[randomNodePosition];

               float randomX = rand() % (int)(this->xMax - this->xMin + 1) ;
               float randomY = rand() % (int)(this->yMax - this->yMin + 1);

               tempNode->setX(randomX + this->xMin);
               tempNode->setY(randomY + this->yMin);

               if (pm->instance().getDebug() == true)
               {
                   qDebug() << neighborMesh.getMeshPointer()[randomNodePosition]->getX() << ": neighborMesh X ";
                   qDebug() << neighborMesh.getMeshPointer()[randomNodePosition]->getY() << ": neighborMesh Y ";
               }

               neighborMesh.setCost(0);
               for (int i = 0 ; i < neighborMesh.getMeshPointer().size() ; i++)
               {
                   Node* tempNode = neighborMesh.getMeshPointer()[i];

                      for (int j = 0 ; j < neighborMesh.getMeshPointer().size() ; j++)
                   {
                       std::vector<Node*> nodeVector = tempNode->getChildNodes();
                       Node* testNode = neighborMesh.getMeshPointer()[j];
               //            qDebug() << tempNode->getY() << "WTF :D.";
                       if(std::find(nodeVector.begin(), nodeVector.end(),  testNode) != nodeVector.end()) {
                           // v contains x
                           if (pm->instance().getDebug() == true)
                           {
                               qDebug() << "contains Element";
                               qDebug("test node : " + testNode->getName().toLatin1());
                               qDebug("temp node : " + tempNode->getName().toLatin1());

                           }

                         int cost = calculateIntersectionCost(innerShape , tempNode , testNode);
                         int currentCost = cost + neighborMesh.getCost();

                         neighborMesh.setCost( currentCost );
                         {
                             qDebug() << "Neighbor Cost is :" << neighborMesh.getCost();
                         }

                       } else {
                           // v does not contain x
                           if (pm->instance().getDebug() == true)
                           {
                               qDebug() << "does not contain element";
                           }


                       }
                   }
               }

               // Get the cities at selected positions in the tour



               // Swap them



               // Get energy of solutions


               // Decide if we should accept the neighbour
//               if (acceptanceProbability(currentEnergy, neighbourEnergy, temp) > Math.random()) {
               if (acceptanceRate(currentMesh.getCost() , neighborMesh.getCost() , temp) > (rand() % 1) && (currentMesh.getCost() != 0))
               {
                    currentMesh = neighborMesh;
               }


               pm->instance().setMesh(neighborMesh);

               // Keep track of the best solution found
               if (currentMesh.getCost() < bestMesh.getCost())
               {
                    bestMesh = currentMesh;
                    if (pm->instance().getDebug() == true)
                    {
                        qDebug() << "acceptance section";
                    }
                    if (pm->instance().getDebug() == true)
                    {
                        qDebug() << "acceptance section";
                    }
                    pm->instance().mainAlgorithmView->update();
                    if (pm->instance().getDebug() == true)
                    {
                        qDebug() << "acceptance section";
                    }
               }

               // Cool system
               //usleep();
               temp *= 1 - coolingRate;
           }

           pm->instance().setMesh(bestMesh);
           pm->instance().mainAlgorithmView->update();
           return bestMesh;
}
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

//=================================================================================================================================
//here we calculate intersection with outerShape and evaluat cost per mesh
int AnealAlgorithm::calculateIntersectionCost(InnerShape innerShape, Node* destinationNode, Node* startNode)
{
    qDebug() <<  "WTF :D.";
    int cost = 0;

    if (destinationNode->getX() == 0 && destinationNode->getY() == 0)
    {
        //Not valid destinationNode (has no position)

    }else if (startNode->getX() == 0 && startNode->getY() == 0)
    {
        //Not valid startNode (has no position).


    }else {

        float destinationNode_X = (float)(destinationNode->getX());
        float destinationNode_Y = (float)(destinationNode->getY()) ;
        float startNode_X = (float)(startNode->getX());

        float startNode_Y = (float)(startNode->getY());

        for (int j = 0 ; j < innerShape.getShapes().size() ; j++)
        {

            std::vector<float> shapes = innerShape.getShapeByIndex(j);

            float startPoint_X = shapes[shapes.size() - 2];
            float startPoint_Y = shapes[shapes.size() - 1];
            float endPoint_X = shapes[0];

            float endPoint_Y = shapes[1];

            qDebug() << "startPoint_X : " << startPoint_X << "startPoint_Y : " << startPoint_Y;
            qDebug() << "endPoint_X : " << endPoint_X << "startPoint_Y : " << endPoint_Y;


            if (areIntersecting( destinationNode_X  , destinationNode_Y , startNode_X , startNode_Y , startPoint_X , startPoint_Y , endPoint_X , endPoint_Y) == NO){

                //here the lines do not intersect so cost = 0
                //so cost stays zero
                if (pm->instance().getDebug() == true)
                {
                    qDebug() << "zero intersection :D";
                }

            }else if (areIntersecting( destinationNode_X  , destinationNode_Y , startNode_X , startNode_Y , startPoint_X , startPoint_Y , endPoint_X , endPoint_Y) == COLLINEAR) {

                //here the line is colinier and line is unaceptebale
                if (pm->instance().getDebug() == true)
                {
                    qDebug() << "the line is collinier deleting this line :D";
                }
                return -1;
            }else if (areIntersecting( destinationNode_X  , destinationNode_Y , startNode_X , startNode_Y , startPoint_X , startPoint_Y , endPoint_X , endPoint_Y) == YES) {

                //here a colision is detected and mesh needs
                cost++;
                if (pm->instance().getDebug() == true)
                {
                    qDebug() << "Creates cost for this class.";
                }
            }




            for (int i = 0 ; i < shapes.size() - 2  ; i = i + 2 )
            {
                qDebug() << shapes << i << j;
                float startPoint_X = shapes[i];
                float startPoint_Y = shapes[i + 1];
                float endPoint_X = shapes[i + 2];

                float endPoint_Y = shapes[i + 3];

                qDebug() << "startPoint_X : " << startPoint_X << "startPoint_Y : " << startPoint_Y;
                qDebug() << "endPoint_X : " << endPoint_X << "startPoint_Y : " << endPoint_Y;

                if (areIntersecting( destinationNode_X  , destinationNode_Y , startNode_X , startNode_Y , startPoint_X , startPoint_Y , endPoint_X , endPoint_Y) == NO){

                    //here the lines do not intersect so cost = 0
                    //so cost stays zero
                    if (pm->instance().getDebug() == true)
                    {
                        qDebug() << "zero intersection :D";
                    }

                }else if (areIntersecting( destinationNode_X  , destinationNode_Y , startNode_X , startNode_Y , startPoint_X , startPoint_Y , endPoint_X , endPoint_Y) == COLLINEAR) {

                    //here the line is colinier and line is unaceptebale
                    if (pm->instance().getDebug() == true)
                    {
                        qDebug() << "the line is collinier deleting this line :D";
                    }
                    return -1;
                }else if (areIntersecting( destinationNode_X  , destinationNode_Y , startNode_X , startNode_Y , startPoint_X , startPoint_Y , endPoint_X , endPoint_Y) == YES) {

                    //here a colision is detected and mesh needs
                    cost++;
                    if (pm->instance().getDebug() == true)
                    {
                        qDebug() << "Creates cost for this class.";
                    }
                }
            }

            if (pm->instance().getDebug() == true)
            {
                qDebug() << "cost is :" << cost;
            }

        }

        return cost;
    }

    if (pm->instance().getDebug() == true)
    {
        qDebug() << "Exit with error.";
    }

    return -1;
}

//=================================================================================================================================
bool AnealAlgorithm::hasIntersectionWithOuterShape(OuterShape outerShape, Node *destinationNode, Node *startNode)
{
    qDebug() <<  "WTF :D.";
    bool intersectionWithOuterShape = false;

    if (destinationNode->getX() == 0 && destinationNode->getY() == 0)
    {
        //Not valid destinationNode (has no position)

    }else if (startNode->getX() == 0 && startNode->getY() == 0)
    {
        //Not valid startNode (has no position).


    }else {

        float destinationNode_X = (float)(destinationNode->getX());
        float destinationNode_Y = (float)(destinationNode->getY()) ;
        float startNode_X = (float)(startNode->getX());

        float startNode_Y = (float)(startNode->getY());

        for (int j = 0 ; j < outerShape.getShapeMatrix().size() ; j++)
        {

            std::vector<float> shapes = outerShape.getShapeMatrix();

            float startPoint_X = shapes[shapes.size() - 2];
            float startPoint_Y = shapes[shapes.size() - 1];
            float endPoint_X = shapes[0];

            float endPoint_Y = shapes[1];

            qDebug() << "startPoint_X : " << startPoint_X << "startPoint_Y : " << startPoint_Y;
            qDebug() << "endPoint_X : " << endPoint_X << "startPoint_Y : " << endPoint_Y;


            if (areIntersecting( destinationNode_X  , destinationNode_Y , startNode_X , startNode_Y , startPoint_X , startPoint_Y , endPoint_X , endPoint_Y) == NO){

                //here the lines do not intersect so cost = 0
                //so cost stays zero
                if (pm->instance().getDebug() == true)
                {
                    qDebug() << "zero intersection :D";
                }


            }else if (areIntersecting( destinationNode_X  , destinationNode_Y , startNode_X , startNode_Y , startPoint_X , startPoint_Y , endPoint_X , endPoint_Y) == COLLINEAR) {

                //here the line is colinier and line is unaceptebale
                intersectionWithOuterShape = true;
                if (pm->instance().getDebug() == true)
                {
                    qDebug() << "the line is collinier deleting this line :D";
                }
                return true;

            }else if (areIntersecting( destinationNode_X  , destinationNode_Y , startNode_X , startNode_Y , startPoint_X , startPoint_Y , endPoint_X , endPoint_Y) == YES) {

                //here a colision is detected and mesh needs
                intersectionWithOuterShape = true;
                if (pm->instance().getDebug() == true)
                {
                    qDebug() << "Creates cost for this class.";
                }

                return true;
            }




            for (int i = 0 ; i < shapes.size() - 2  ; i = i + 2 )
            {
                qDebug() << shapes << i << j;
                float startPoint_X = shapes[i];
                float startPoint_Y = shapes[i + 1];
                float endPoint_X = shapes[i + 2];

                float endPoint_Y = shapes[i + 3];

                qDebug() << "startPoint_X : " << startPoint_X << "startPoint_Y : " << startPoint_Y;
                qDebug() << "endPoint_X : " << endPoint_X << "startPoint_Y : " << endPoint_Y;

                if (areIntersecting( destinationNode_X  , destinationNode_Y , startNode_X , startNode_Y , startPoint_X , startPoint_Y , endPoint_X , endPoint_Y) == NO){

                    //here the lines do not intersect so cost = 0
                    //so cost stays zero
                    if (pm->instance().getDebug() == true)
                    {
                        qDebug() << "zero intersection :D";
                    }

                }else if (areIntersecting( destinationNode_X  , destinationNode_Y , startNode_X , startNode_Y , startPoint_X , startPoint_Y , endPoint_X , endPoint_Y) == COLLINEAR) {

                    //here the line is colinier and line is unaceptebale
                    if (pm->instance().getDebug() == true)
                    {
                        qDebug() << "the line is collinier deleting this line :D";
                    }

                    return true;

                }else if (areIntersecting( destinationNode_X  , destinationNode_Y , startNode_X , startNode_Y , startPoint_X , startPoint_Y , endPoint_X , endPoint_Y) == YES) {

                    //here a colision is detected and mesh needs

                    if (pm->instance().getDebug() == true)
                    {
                        qDebug() << "Creates cost for this class.";
                    }
                    return true;
                }
            }
        }

        return false;
    }

    if (pm->instance().getDebug() == true)
    {
        qDebug() << "Exit with error.";
    }

    return false;
}






