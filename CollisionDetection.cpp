#include <iostream>
#include <vector>
#include <stdarg.h>
#include <string>
#include <stack>
#include <math.h>

using namespace std;

// Punto unidimensional
class Point {
    public:
        int x;
        int y;
        Point (int x, int y) {
            this->x=x;
            this->y=y;
        }
};

// Figura bidimencional
class Shape {
    public:
        vector<Point> vertex;
        Shape (int n, ...) {
            int x, y;
            va_list vl;
            va_start(vl,n);
            for (int i=0;i<n;i++)
            {
                x=va_arg(vl, int);
                y=va_arg(vl, int);
                vertex.push_back(Point(x, y));
            }
            va_end(vl);
        }
        Shape () {}
};

// Rectangulo que representa un cuadrante en el espacio
class AABB {
    public:
        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;
};

// Nodo del arbol, puede representar a una figura
// o usarse para contener figuras y cuadrantes.
class Node {
    public:
        static int currentID;
        int id;
        AABB aabb;
        Shape shape;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;

        Node (AABB aabb): id(currentID++) {
            this->aabb = aabb;
        }

        Node (AABB aabb, Shape shape): id(currentID++) {
            this->aabb = aabb;
            this->shape = shape;
        }

        bool isContainer () { return this->shape.vertex.size() == 0; } // comprueba si el nodo es un contenedor o representa a una figura

        string toString (); // imprime el AABB del nodo

        bool checkCollision (AABB aabb);
};
int Node::currentID; // permite tener ID's progresivos 0, 1, 2, 3, 4...

// Arbol, contiene los metodos y el nodo raiz
class AABBTree {
    public:
        Node* rootNode = nullptr;
        void addShape(Shape shape);
        Node* checkCollision(Shape shape);
        AABB getAABBfromShape(Shape shape);

    private:
        AABB getAABBfromNodes(Node* node1, Node* node2);
        Node* getBestPartner(Node* node);
        int getDistance(Node* node1, Node* node2);
};

int main () {
    AABBTree tree;
    Shape shape(4, 0, 0, 1, 0, 1, 1, 0, 1); // cuadrado de lado 1
    tree.addShape(shape);
    Shape shape2(4, 1, 1, 2, 1, 2, 2, 1, 2); // otro cuadrado de lado 1 jaja xD
    tree.addShape(shape2);
    Shape shape3(4, 1, -1, 0, -1, 0, 0, -1, 0); // y contra todo pronostico un tercer cuadrado de lado 1
    tree.addShape(shape3);
    // Deteccion de collision
    Shape shape4(4, 0, 0, 0, 2, 2, 2, 0, 2);
    Node* collision = tree.checkCollision(shape4);
    if (collision)
        cout << "Se detecto una colision con el siguiente nodo:\n" << collision->toString();
}

// Agregar figura al arbol
void AABBTree::addShape(Shape shape) {
    Node* node = new Node(this->getAABBfromShape(shape), shape);

    if (this->rootNode == nullptr)
        this->rootNode = node;
    else {
        Node* partner = this->getBestPartner(node);
        Node* containerNode = new Node(this->getAABBfromNodes(node, partner));

        containerNode->left = node;
        containerNode->right = partner;

        if (partner->parent == nullptr)
            this->rootNode = containerNode;
        else {
            Node* partnerParent = partner->parent;
            if (partnerParent->left != nullptr && partnerParent->left->id == partner->id)
                partnerParent->left = containerNode;
            else
                partnerParent->right = containerNode;

            // actualizar padres
            while (partnerParent != nullptr) {
                partnerParent->aabb = this->getAABBfromNodes(partnerParent->left, partnerParent->right);
                partnerParent = partnerParent->parent;
            }
        }

        partner->parent = containerNode;
        node->parent = containerNode;
    }

    return;
}

// Obtener cuadrante AABB de una figura de N vertices
AABB AABBTree::getAABBfromShape(Shape shape) {
    AABB aabb;

    for (long unsigned i=0; i < shape.vertex.size(); i++) {
        Point vertex = shape.vertex[i];
        if (vertex.x < aabb.x1) aabb.x1 = vertex.x;
        if (vertex.y < aabb.y1) aabb.y1 = vertex.y;
        if (vertex.x > aabb.x2) aabb.x2 = vertex.x;
        if (vertex.y > aabb.y2) aabb.y2 = vertex.y;
    }

    return aabb;
}

// Obtiene un cuadrante AABB a partir de otros cuadrantes
AABB AABBTree::getAABBfromNodes(Node* node1, Node* node2) {
    AABB aabb1 = node1->aabb;
    AABB aabb2 = node2->aabb;
    AABB aabb;
    vector<int> xs;
    xs.push_back(aabb1.x1);
    xs.push_back(aabb1.x2);
    xs.push_back(aabb2.x1);
    xs.push_back(aabb2.x2);
    vector<int> ys;
    ys.push_back(aabb1.y1);
    ys.push_back(aabb1.y2);
    ys.push_back(aabb2.y1);
    ys.push_back(aabb2.y2);
    
    for (long unsigned i=0; i < xs.size(); i++) {
        if (xs[i] < aabb.x1) aabb.x1 = xs[i];
        if (ys[i] < aabb.y1) aabb.y1 = ys[i];
        if (xs[i] > aabb.x2) aabb.x2 = xs[i];
        if (ys[i] > aabb.y2) aabb.y2 = ys[i];
    }

    return aabb;
}

// Retorna el mejor compañero para emparejar un nodo
Node* AABBTree::getBestPartner(Node* node) {
    if (this->rootNode == nullptr) return nullptr;
    // Comprobamos si es que el nuevo nodo ya esta collisionando
    Node* best = this->checkCollision(node->shape);
    if (best) return best;

    best = this->rootNode;
    // Recorremos el arbol en preorden, mejorable
    stack<Node*> recorrido;
    recorrido.push(this->rootNode);
    while (!recorrido.empty()) {
        Node* visitando = recorrido.top();
        recorrido.pop();
        if (this->getDistance(visitando, node) < this->getDistance(node, best))
            best = visitando;
        if (node->right != nullptr)
            recorrido.push(node->left);
        if (node->left != nullptr)
            recorrido.push(node->right);
    }
    return best;
}

// Obtiene la distancia entre 2 AABB a partir de sus puntos medios
int AABBTree::getDistance(Node* node1, Node* node2) {
    AABB aabb1 = node1->aabb;
    AABB aabb2 = node2->aabb;
    int middlePointX1 = (aabb1.x1 + aabb1.x2)/2; 
    int middlePointY1 = (aabb1.y1 + aabb1.y2)/2;
    int middlePointX2 = (aabb2.x1 + aabb2.x2)/2; 
    int middlePointY2 = (aabb2.y1 + aabb2.y2)/2;

    return sqrt(pow(middlePointX2-middlePointX1, 2) + pow(middlePointY2-middlePointY1, 2));
}


string Node::toString () {
    string nodestr = "Node {\n AABB: {\n";
    nodestr += "  x1: " + to_string(this->aabb.x1) + string(", ");
    nodestr += "y1: " + to_string(this->aabb.y1) + string(", ");
    nodestr += "x2: " + to_string(this->aabb.x2) + string(", ");
    nodestr += "y2: " + to_string(this->aabb.y2) + string("\n");
    nodestr += " }\n}\n";
    return nodestr;
}

Node* AABBTree::checkCollision(Shape shape) {
    AABB aabb = this->getAABBfromShape(shape);
    stack<Node*> nodes;
    nodes.push(this->rootNode);
    
    while (!nodes.empty()) {
        Node* node = nodes.top();
        nodes.pop();
        if (node->checkCollision(aabb)) {
            cout << "Tree: se detecto una collision interna\n";
            if (node->isContainer()) {
                nodes.push(node->left);
                nodes.push(node->right);
            } else return node;
        }
    }

    return nullptr;
}

bool Node::checkCollision (AABB aabb) {
    return
        (this->aabb.y1 - aabb.y2)*(aabb.y1 - this->aabb.y2) > 0 &&
        (aabb.x2 - this->aabb.x1)*(this->aabb.x2 - aabb.x1) > 0;
}

