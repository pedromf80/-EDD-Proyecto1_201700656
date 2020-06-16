/* 
 * File:   Reporte.cpp
 * Author: pedro
 *
 * Created on June 5, , 00:50 AM
*/
#include "Reporte.h"
#include "TAVL.h"
#include "Usuario.h"
#include <string>

using std::string;

Reporte::Reporte()
{
}

//reporte de lista doblemente circular
void Reporte::ReporteListaDobleCircular(ListaDoble<Transaccion *> *lista_circular, string nameDot)
{
    Graphviz *graph = new Graphviz();
    graph->addln(graph->start_graph());
    graph->addln("rankdir=LR;");
    graph->addln("node [shape=record, color=blue, width=0.5, height=0.5]; ");
    graph->addln();
    int contador = 0, size = lista_circular->getSize();
    Node<Transaccion *> *primero = lista_circular->getPrimero();
    string nodos, enlaces, enlacesIverso;
    if (lista_circular != 0)
    {
        Node<Transaccion *> *aux = primero;
        do
        {
            if (contador < size - 1)
            {
                nodos = nodos + "node" + to_string(contador) + " [label=\"{<a>|ID T: " + aux->getData()->getIdT() + "\\nID A: " + aux->getData()->getIdActivo() + "\\nUsuario: " + aux->getData()->getNombreUsuario() + "\\nTiempo: " + to_string(aux->getData()->getTiempo()) + " dias|<b>}\"];\n";
                enlaces = enlaces + "node" + to_string(contador) + ":b:c -> node" + to_string(contador + 1) + ":a:c [arrowtail=dot, dir=both,tailclip=false];\n";
                enlacesIverso = enlacesIverso + "node" + to_string(contador + 1) + ":a:c -> node" + to_string(contador) + ":b:c [arrowtail=dot, dir=both,tailclip=false];\n";
            }
            else
            {
                nodos = nodos + "node" + to_string(contador) + " [label=\"{<a>|ID T: " + aux->getData()->getIdT() + "\\nID A: " + aux->getData()->getIdActivo() + "\\nUsuario: " + aux->getData()->getNombreUsuario() + "\\nTiempo: " + to_string(aux->getData()->getTiempo()) + " dias|<b>}\"];\n";
                enlaces = enlaces + "node" + to_string(contador) + ":b:c -> node" + to_string(0) + ":a:c [arrowtail=dot, dir=both,tailclip=false];\n";
                enlacesIverso = enlacesIverso + "node" + to_string(0) + ":a:c -> node" + to_string(contador) + ":b:c [arrowtail=dot, dir=both,tailclip=false];\n";
            }
            contador++;
            aux = aux->getNext();
        } while (aux != primero);
    }
    graph->addln(nodos);
    graph->addln(enlaces);
    graph->addln(enlacesIverso);
    graph->addln(graph->end());
    graph->dotGraphGenerator(nameDot, graph->getDotSource());
    graph->setDotSource();
}

//reporte del arbol avl metodo privado por recursividad
void Reporte::reporteArbolAvl(NodeAvl<Activo *, string> *raiz, Graphviz *graph)
{
    static int count = 0;
    if (raiz != nullptr)
    {
        if (raiz->getIzquierda())
        {
            graph->add(("\"" + raiz->getLlave()) + "\"[label=\"" + raiz->getDato()->getIdActivo() + "\\n" + raiz->getDato()->getNombre() + "\\nDisponible = " + getDisponible(raiz->getDato()->getDisponibilidad()) + "\"];\n");
            graph->addln("\"" + (raiz->getLlave()) + "\" -> \"" + (raiz->getIzquierda()->getLlave()) + "\";");
            reporteArbolAvl(raiz->getIzquierda(), graph);
        }
        else
        {
            graph->add(("\"" + raiz->getLlave()) + "\"[label=\"" + raiz->getDato()->getIdActivo() + "\\n" + raiz->getDato()->getNombre() + "\\nDisponible = " + getDisponible(raiz->getDato()->getDisponibilidad()) + "\"]\n");
            graph->addln("null" + to_string(count) + " [shape=point];");
            graph->addln(("\"" + raiz->getLlave()) + "\" -> null" + to_string(count) + ";");
            count++;
        }
        if (raiz->getDerecha())
        {
            graph->addln(("\"" + raiz->getLlave()) + "\" -> \"" + (raiz->getDerecha()->getLlave()) + "\";");
            reporteArbolAvl(raiz->getDerecha(), graph);
        }
        else
        {
            graph->addln("null" + to_string(count) + " [shape=point];");
            graph->addln(("\"" + raiz->getLlave()) + "\" -> null" + to_string(count) + ";");
            count++;
        }
    }
}

//metodo publico para el reporte del arbol avl
void Reporte::reporteAVL(NodeAvl<Activo *, string> *raiz)
{
    Graphviz *graph = new Graphviz();
    graph->addln(graph->start_graph());
    graph->addln("node[fontname=\"Arial\", color=\"blue\"]");
    graph->addln("edge [color=\"green\"]");
    reporteArbolAvl(raiz, graph);
    graph->addln(graph->end());
    graph->dotGraphGenerator("ArbolAVL", graph->getDotSource());
    graph->setDotSource();
}

//retorna si esta disponible o no
string Reporte::getDisponible(bool caso)
{
    if (caso)
        return "si";
    else
        return "no";
}

//metodo inorden para el arbol avl imprimir datos para su eliminacion
void Reporte::inorden(NodeAvl<Activo *, string> *root)
{
    if (root)
    {
        inorden(root->getIzquierda());
        cout << "ID: " << root->getLlave() << ";\tNombre: " << root->getDato()->getNombre() << "\n";
        inorden(root->getDerecha());
    }
}

//metodo preorden par el arbol avl imprimir datos para su eliminacion
void Reporte::preorden(NodeAvl<Activo *, string> *root)
{
    if (root)
    {
        cout << "ID: " << root->getLlave() << ";\tNombre: " << root->getDato()->getNombre() << "\n";
        preorden(root->getIzquierda());
        preorden(root->getDerecha());
    }
}

//metodo que me imprime el catalogo de activos recibe como parametro una lista de usuarios
void Reporte::printCatalogoActivos(Lista<Usuario *> *listCatalogo)
{
    for (size_t i = 0; i < listCatalogo->size(); i++)
    {
        Usuario *tmpus = listCatalogo->getData();
        printCatalgo(tmpus->getArbolAvl()->getRaiz());
    }
}

//metodo privado donde se imprime el catologo de los disponibles de acada usuario
void Reporte::printCatalgo(NodeAvl<Activo *, string> *root)
{
    if (root)
    {
        if (root->getDato()->getDisponibilidad())
            cout << "ID: " << root->getLlave() << ";\t Nombre: " << root->getDato()->getNombre() << ";\t\t Tiempo de Renta: " << root->getDato()->getTiempo() << "\n";
        printCatalgo(root->getIzquierda());
        printCatalgo(root->getDerecha());
    }
}

//metodo que devuelve un activo con el id espesificado
Activo *Reporte::getActivoRenta(Lista<Usuario *> *listU, string id)
{
    for (size_t i = 0; i < listU->size(); i++)
    {
        Usuario *utmp = listU->getData();
        NodeAvl<Activo *, string> *ntmp = utmp->getArbolAvl()->buscar(id);
        if (ntmp)
            return ntmp->getDato();
    }
    return nullptr;
}

//metodo que imprime los activos rentados de un usuario
void Reporte::printMisActivosRentados(NodeAvl<Activo *, string> *root)
{
    if (root)
    {
        if (!root->getDato()->getDisponibilidad())
            cout << "ID: " << root->getLlave() << ";\t Nombre: " << root->getDato()->getNombre() << ";\t\t Tiempo de Renta: " << root->getDato()->getTiempo() << "\n";
        printMisActivosRentados(root->getIzquierda());
        printMisActivosRentados(root->getDerecha());
    }
}

//destructor
Reporte::~Reporte()
{
}