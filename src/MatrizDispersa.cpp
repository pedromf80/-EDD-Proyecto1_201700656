
#include <stdlib.h>
#include <string>
#include <iostream>
#include "MatrizDispersa.h"

using std::string;
MatrizDispersa::MatrizDispersa()
{
    root = new NodeM("ini_", "ini_y", new Usuario("admin", "admin"));
    graph = new Graphviz();
}

void MatrizDispersa::insertMatrix(string x, string y, Usuario *usuario)
{
    //nodos temporales para comprobaciones e insercion
    NodeM *tmpY = search_Row_Column(root->getDown(), y);
    NodeM *tmpX = search_Row_Column(root->getRight(), x);

    //nuevo nodo de new_node
    NodeM *new_node = new NodeM(x, y, usuario);

    if (tmpY == nullptr && tmpX == nullptr)
    {
        NodeM *newX = new NodeM(x, "", 0);
        NodeM *newY = new NodeM("", y, 0);
        insertSortRow(newY, y);
        insertSortColumn(newX, x);
        insertNode(newX, newY, new_node);
    }
    else if (tmpY == nullptr && tmpX != nullptr)
    {
        NodeM *newY = new NodeM("", y, 0);
        insertSortRow(newY, y);
        insertNode(tmpX, newY, new_node);
    }
    else if (tmpY != nullptr && tmpX == nullptr)
    {
        NodeM *newX = new NodeM(x, "", 0);
        insertSortColumn(newX, x);
        insertNode(newX, tmpY, new_node);
    }
    else if (tmpY != nullptr && tmpX != nullptr)
    {
        insertNode(tmpX, tmpY, new_node);
    }
}

//metodo privado insertar de forma ordenada en la columna
void MatrizDispersa::insertSortColumn(NodeM *newX, string x)
{
    //  NodeM* newX = new NodeM(x,0,"");
    NodeM *tmp = root;
    if (isEmpty())
    {
        tmp->setRight(newX);
        newX->setLeft(tmp);
        sizeX++;
    }
    else
    {
        while (tmp->getRight() != nullptr && tmp->getRight()->getX() < x)
        {
            tmp = tmp->getRight();
        }
        if (tmp->getRight() != nullptr)
        {
            newX->setRight(tmp->getRight());
            tmp->getRight()->setLeft(newX);
            tmp->setRight(newX);
            newX->setLeft(tmp);
            sizeX++;
        }
        else
        {
            tmp->setRight(newX);
            newX->setLeft(tmp);
            sizeX++;
        }
    }
}

//metodo privado para insertar ordenado en y
void MatrizDispersa::insertSortRow(NodeM *newY, string y)
{
    //  NodeM* newY = new NodeM(0,y,"");
    NodeM *tmp = root;
    if (isEmpty())
    {
        tmp->setDown(newY);
        newY->setUp(tmp);
        sizeY++;
    }
    else
    {
        while (tmp->getDown() != nullptr && tmp->getDown()->getY() < y)
        {
            tmp = tmp->getDown();
        }
        if (tmp->getDown() != nullptr)
        {
            newY->setDown(tmp->getDown());
            tmp->getDown()->setUp(newY);
            tmp->setDown(newY);
            newY->setUp(tmp);
            sizeY++;
        }
        else
        {
            tmp->setDown(newY);
            newY->setUp(tmp);
            sizeY++;
        }
    }
}

//devuelve un bool cuando la matriz no tiene comlumnas ni filas
bool MatrizDispersa::isEmpty()
{
    if (root->getRight() == nullptr && root->getDown() == nullptr)
    {
        return true;
    }
    return false;
}

//metodo privado para imprimir la matriz
void MatrizDispersa::show(NodeM *n)
{
    if (isEmpty())
    {
        std::cout << "matriz vacia\n";
    }
    else
    {
        if (n == root->getRight())
        {
            while (n != nullptr)
            {
                std::cout << n->getX() << "\n";
                n = n->getRight();
            }
        }
        else if (n == root->getDown())
        {
            while (n != nullptr)
            {
                std::cout << n->getY() << "\n";
                n = n->getDown();
            }
        }
    }
}

//metodo publico para imprimir columnaX temporal para debug
void MatrizDispersa::showX()
{
    if (!isEmpty())
    {
        show(root->getRight());
    }
}
//metodo publico para imprimir filaY temporal para debug
void MatrizDispersa::showY()
{
    if (!isEmpty())
    {
        show(root->getDown());
    }
}

//devuelve booleano verdadero si la columna o fila ya existe para no crearse de nuevo
NodeM *MatrizDispersa::search_Row_Column(NodeM *n, string position)
{
    if (root->getRight() == n)
    {
        while (n != nullptr)
        {
            if (n->getX() == position)
            {
                return n;
            }
            n = n->getRight();
        }
        return nullptr;
    }
    else if (root->getDown() == n)
    {
        while (n != nullptr)
        {
            if (n->getY() == position)
            {
                return n;
            }
            n = n->getDown();
        }
        return nullptr;
    }
}

//insertar en el indice indicado
void MatrizDispersa::insertNode(NodeM *x, NodeM *y, NodeM *new_node)
{
    if (x->getDown() == nullptr && y->getRight() == nullptr)
    {
        x->setDown(new_node);
        new_node->setUp(x);
        y->setRight(new_node);
        new_node->setLeft(y);
        // std::cout <<x->getDown()->getY()<<" "<<x->getDown()->getX()<<"\n";
    }
    else if (x->getDown() != nullptr && y->getRight() == nullptr)
    {
        NodeM *tmpXposition = lastNodeColumn(x);
        new_node->setUp(tmpXposition);
        tmpXposition->setDown(new_node);
        y->setRight(new_node);
        new_node->setLeft(y);
        // std::cout << tmpXposition->getX() << " " << tmpXposition->getY() <<"\t"<<tmpXposition->getData()<< "\n";
    }
    else if (x->getDown() == nullptr && y->getRight() != nullptr)
    {
        NodeM *tmpYposition = lastNodeRow(y);
        new_node->setLeft(tmpYposition);
        tmpYposition->setRight(new_node);
        x->setDown(new_node);
        new_node->setUp(x);
        // std::cout << tmpYposition->getX() << " " << tmpYposition->getY() <<"\t"<<tmpYposition->getData()<< "\n";
    }
    else if (x->getDown() != nullptr && y->getRight() != nullptr)
    {
        //comprobar si la posicion ya esta ocupada
        if (!posxyBusy(x, new_node->getX(), new_node->getY()))
        {
            NodeM *Xposition = NodeColumnMed(x, new_node->getY());
            NodeM *Yposition = NodeRowMed(y, new_node->getX());
            //cout << NodeColumnMed(x, new_node->getY())->getData() << "\n";
            //cout << NodeRowMed(y, new_node->getX())->getData() << "\n";
            //caso de inserccion cuando los nodos devueltos son los ultimos
            if (Xposition->getDown() == nullptr && Yposition->getRight() == nullptr)
            {
                new_node->setUp(Xposition);
                Xposition->setDown(new_node);
                new_node->setLeft(Yposition);
                Yposition->setRight(new_node);
            }
            else if (Xposition->getDown() == nullptr && Yposition->getRight() != nullptr)
            {
                //punteros en vertical cuando el nodo devuelto es el ultimo
                new_node->setUp(Xposition);
                Xposition->setDown(new_node);
                //puntero en horizontal cuando el nodo devuelto esta en medio de nodos
                new_node->setRight(Yposition->getRight());
                new_node->setLeft(Yposition);
                Yposition->getRight()->setLeft(new_node);
                Yposition->setRight(new_node);
            }
            else if (Xposition->getDown() != nullptr && Yposition->getRight() == nullptr)
            {
                //el ultimo puntero devuelto en horizontal es el ultimo
                new_node->setLeft(Yposition);
                Yposition->setRight(new_node);

                //cambio de punteros cuando el nodo devuelto en vertical esta entre nodos
                new_node->setUp(Xposition);
                new_node->setDown(Xposition->getDown());
                Xposition->getDown()->setUp(new_node);
                Xposition->setDown(new_node);
            }
            else if (Xposition->getDown() != nullptr && Yposition->getRight() != nullptr)
            {
                //puntero en horizontal cuando el nodo devuelto esta en medio de nodos
                new_node->setRight(Yposition->getRight());
                new_node->setLeft(Yposition);
                Yposition->getRight()->setLeft(new_node);
                Yposition->setRight(new_node);

                //cambio de punteros cuando el nodo devuelto en vertical esta entre nodos
                new_node->setUp(Xposition);
                new_node->setDown(Xposition->getDown());
                Xposition->getDown()->setUp(new_node);
                Xposition->setDown(new_node);
            }
        }
        else
        {
            NodeM *Xposition = NodeColumnMed(x, new_node->getY());
            NodeM *Yposition = NodeRowMed(y, new_node->getX());
            if (Xposition->getX() == new_node->getX() && Yposition->getY() == new_node->getY())
            {
                NodeM *tmp = recorridoProfundidad(Xposition);
                if (!(tmp->getData()->getNombre() == new_node->getData()->getNombre()))
                {
                    tmp->setBehind(new_node);
                    new_node->setFront(tmp);
                }else
                    cout<<"usuario "<<tmp->getData()->getNombre()<<" ya existe!!\n";
            }
        }
    }
}

//metodo para insertar donde ya existen usuarios en la matriz y decir si estan repetidos
NodeM *MatrizDispersa::recorridoProfundidad(NodeM *n)
{
    while (n->getBehind() != nullptr)
    {
        n = n->getBehind();
    }
    return n;
}

//metodo para insertar en la matriz en medio de nodos devuelve una posicion anterior
NodeM *MatrizDispersa::NodeColumnMed(NodeM *n, string y)
{
    while (n != nullptr)
    {
        if (n->getY() > y && n->getDown() == nullptr)
        {
            return n->getUp();
        }
        else if (n->getY() > y && n->getDown() != nullptr)
        {
            return n->getUp();
        }
        else if (n->getDown() == nullptr)
        {
            return n;
        }
        n = n->getDown();
    }
}

NodeM *MatrizDispersa::NodeRowMed(NodeM *n, string x)
{
    while (n != nullptr)
    {
        if (n->getX() > x && n->getRight() == nullptr)
        {
            return n->getLeft();
        }
        else if (n->getX() > x && n->getRight() != nullptr)
        {
            return n->getLeft();
        }
        else if (n->getRight() == nullptr)
        {
            return n;
        }
        n = n->getRight();
    }
}

//metodo recursivo que devuelve el ultimo nodo en columna por posicion
NodeM *MatrizDispersa::lastNodeColumn(NodeM *n)
{
    while (n->getDown() != nullptr)
    {
        n = n->getDown();
    }
    return n;
}

//metodo que devuelve el ultimo nodo en la fila por posicion
NodeM *MatrizDispersa::lastNodeRow(NodeM *n)
{
    while (n->getRight() != nullptr)
    {
        n = n->getRight();
    }
    return n;
}
//metodo que devuelven el tamanio de filas y columnas
int MatrizDispersa::getSizeX() { return sizeX; }
int MatrizDispersa::getSizeY() { return sizeY; }

//mostrar cotenido de la matriz
void MatrizDispersa::showMatrix()
{
    NodeM *tempY = root->getDown();
    NodeM *tempX;
    if (!isEmpty())
    {
        while (tempY != NULL)
        {
            tempX = tempY->getRight();
            while (tempX != NULL)
            {

                std::cout << tempX->getData() << "->";
                tempX = tempX->getRight();
            }
            std::cout << "\n";
            tempY = tempY->getDown();
        }
    }
}

//genera un reporte en graphviz
void MatrizDispersa::report()
{
    graph->addln(graph->start_graph());
    graph->addln("rankdir = TB;");
    graph->addln("node [shape=rectangle, color=blue, height=0.5, width=0.5];");
    graph->addln("edge [color= red];");
    graph->addln("graph[ nodesep = 0.5];");
    graph->addln("nodeXY [label=\"Emp\\\\Dep\"];");
    NodeM *tempY = root->getDown();
    NodeM *tempX = root->getRight();
    NodeM *aux, *aux2;
    if (!isEmpty())
    {
        //recorrer encabezado X para porder agregarlos al dot
        string rankSameX = "{rank=same; nodeXY; ";
        string nodesY = "", nodesYp = "", nodesX = "", nodesXp = "", nodeV = "", nodeVp = "";
        nodesXp = nodesXp + "nodeXY -> nodeX" + tempX->getX() + " [dir=both];\n";
        while (tempX != nullptr)
        {
            //enlace hacia los nodos directos desde la columna
            nodeVp = nodeVp + "nodeX" + (tempX->getX()) + " -> nodev" + (tempX->getDown()->getX()) + (tempX->getDown()->getY()) + " [dir=both];\n";
            //------------------------------------------------------------------------------------------------------------------------------------------------------------------
            nodesX = nodesX + "nodeX" + (tempX->getX()) + " [label=\"" + (tempX->getX()) + "\"];\n";
            rankSameX = rankSameX + "nodeX" + (tempX->getX()) + "; ";
            aux2 = tempX->getDown();
            if (tempX->getRight() != nullptr)
            {
                nodesXp = nodesXp + "nodeX" + (tempX->getX()) + " -> nodeX" + (tempX->getRight()->getX()) + " [dir=both];\n";
                //codigo para enlazar nodos mas internos
                while (aux2 != nullptr)
                {
                    if (aux2->getDown() != nullptr)
                    {
                        nodeVp = nodeVp + "nodev" + (aux2->getX()) + (aux2->getY()) + " -> nodev" + (aux2->getDown()->getX()) + (aux2->getDown()->getY()) + " [dir=both];\n";
                    }
                    aux2 = aux2->getDown();
                }
            }
            else
            {
                //ultima columna
                while (aux2 != nullptr)
                {
                    if (aux2->getDown() != nullptr)
                    {
                        nodeVp = nodeVp + "nodev" + (aux2->getX()) + (aux2->getY()) + " -> nodev" + (aux2->getDown()->getX()) + (aux2->getDown()->getY()) + " [dir=both];\n";
                    }
                    aux2 = aux2->getDown();
                }
            }
            tempX = tempX->getRight();
        }
        rankSameX = rankSameX + "}\n";
        //recorrer encabezado Y para poder agregarlos al dot
        nodesYp = nodesYp + "nodeXY -> nodeY" + (tempY->getY()) + " [dir=both];\n";
        while (tempY != nullptr)
        {
            nodesY = nodesY + "nodeY" + (tempY->getY()) + " [label=\"" + (tempY->getY()) + "\"];\n";
            aux = tempY->getRight(); //auxiliar para producir nodos
            if (tempY->getDown() != nullptr)
            {
                //genera los nodos dot de la columna (y)
                nodesYp = nodesYp + "nodeY" + (tempY->getY()) + " -> nodeY" + (tempY->getDown()->getY()) + " [dir=both];\n";
                nodeVp = nodeVp + "nodeY" + (tempY->getY()) + " -> nodev" + (aux->getX()) + (aux->getY()) + " [constraint=false, dir=both];\n";
                if (aux->getRight() != nullptr)
                {
                    //genera todos los nodos
                    nodeV = nodeV + "nodev" + (aux->getX()) + (aux->getY()) + " [label=\"" + aux->getData()->getNombre() + "\"];\n";
                    while (aux != nullptr)
                    {
                        if (aux->getRight() != nullptr)
                        {
                            nodeV = nodeV + "nodev" + (aux->getRight()->getX()) + (aux->getRight()->getY()) + " [label=\"" + aux->getRight()->getData()->getNombre() + "\"];\n";
                            nodeVp = nodeVp + "nodev" + (aux->getX()) + (aux->getY()) + " -> nodev" + (aux->getRight()->getX()) + (aux->getRight()->getY()) + " [constraint=false, dir=both];\n";
                        }
                        aux = aux->getRight();
                    }
                }
                else
                {
                    nodeV = nodeV + "nodev" + (aux->getX()) + (aux->getY()) + " [label=\"" + aux->getData()->getNombre() + "\"];\n";
                }
            }
            else
            { //para no saltarse el ultimo nodo
                if (aux != nullptr)
                {
                    //genera todos los nodos que estan de ultimo y se los salta por la condicional
                    nodeVp = nodeVp + "nodeY" + (tempY->getY()) + " -> nodev" + (aux->getX()) + (aux->getY()) + " [constraint=false, dir=both];\n";
                    nodeV = nodeV + "nodev" + (aux->getX()) + (aux->getY()) + " [label=\"" + aux->getData()->getNombre() + "\"];\n";
                    while (aux != nullptr)
                    {
                        if (aux->getRight() != nullptr)
                        {
                            nodeV = nodeV + "nodev" + (aux->getRight()->getX()) + (aux->getRight()->getY()) + " [label=\"" + aux->getRight()->getData()->getNombre() + "\"];\n";
                            nodeVp = nodeVp + "nodev" + (aux->getX()) + (aux->getY()) + " -> nodev" + (aux->getRight()->getX()) + (aux->getRight()->getY()) + " [constraint=false, dir=both];\n";
                        }
                        aux = aux->getRight();
                    }
                }
            }
            tempY = tempY->getDown();
        }
        graph->addln(nodesX);
        graph->addln(nodesY);
        graph->addln(nodeV);
        graph->addln(nodesXp);
        graph->addln(nodesYp);
        graph->addln(nodeVp);
        graph->add(rankSameX);
        graph->add(sameX());
    }
    graph->addln(graph->end());
    graph->dotGraphGenerator("matriz", graph->getDotSource());
    graph->setDotSource();
}

//revuelvel el rank dir para reporte
string MatrizDispersa::sameX()
{
    NodeM *tempY = root->getDown();
    NodeM *tempX;
    string rankSameX;
    if (!isEmpty())
    {
        while (tempY != NULL)
        {
            rankSameX = rankSameX + "{rank=same; ";
            rankSameX = rankSameX + "nodeY" + (tempY->getY()) + "; ";
            tempX = tempY->getRight();
            while (tempX != NULL)
            {
                rankSameX = rankSameX + "nodev" + (tempX->getX()) + (tempX->getY()) + "; ";
                tempX = tempX->getRight();
            }
            rankSameX = rankSameX + "}\n";
            tempY = tempY->getDown();
        }
    }
    return rankSameX;
}

bool MatrizDispersa::posxyBusy(NodeM *n, string x, string y)
{
    while (n != nullptr)
    {
        if (n->getY() == y && n->getX() == x)
        {
            return true;
        }
        n = n->getDown();
    }
    return false;
}

//busqueda en la matriz devuelve el dato almacenado dados las posiciones
Usuario *MatrizDispersa::searchM(string x, string y)
{
    NodeM *tempY = root->getDown();
    NodeM *tempX;
    if (!isEmpty())
    {
        while (tempY != NULL)
        {
            tempX = tempY->getRight();
            while (tempX != NULL)
            {
                if (tempX->getX() == x && tempY->getY() == y)
                {
                    return tempX->getData();
                }

                tempX = tempX->getRight();
            }
            tempY = tempY->getDown();
        }
    }
}

//cambia el valor del dato dentro de la matriz en la posicin indicada
void MatrizDispersa::setData(string x, string y, Usuario *data_)
{
    NodeM *tempY = root->getDown();
    NodeM *tempX;
    if (!isEmpty())
    {
        while (tempY != NULL)
        {
            tempX = tempY->getRight();
            while (tempX != NULL)
            {
                if (tempX->getX() == x && tempY->getY() == y)
                {
                    tempX->setData(data_);
                }

                tempX = tempX->getRight();
            }
            tempY = tempY->getDown();
        }
    }
}

//devuelve el usuario administrador creado al inicializar la matriz
Usuario *MatrizDispersa::getAdmin(){return root->getData();} 

//destructor
MatrizDispersa::~MatrizDispersa()
{
}
