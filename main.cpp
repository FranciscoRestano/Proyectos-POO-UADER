#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
using namespace std;

class Curso {
protected:
    string codigo;
    double horas;
    string nombre;
    string tema;
public:
    Curso (string cod, double h, string n, string t);

    double getHoras () {return horas;}
    string getNombre () {return nombre;}
    string getTema () {return tema;}

    virtual void calcularHoras () = 0;
};

Curso::Curso (string cod, double h, string n, string t) {
    codigo = cod;
    horas = h;
    nombre = n;
    tema = t;
}

class Simple : public Curso {
public:
    Simple (string cod, double h, string n, string t) : Curso (cod, h, n, t) {}

    void calcularHoras();
};

void Simple::calcularHoras() {
    // no pongo nada porque no hace nada, las horas la ingresan
}

class Correlativa : public Curso {
private:
    vector <Simple*> matCorrelativas;
public:
    Correlativa (string cod, string n, string t) : Curso (cod, 0.0, n, t) {}

    void addCorrelativa (Simple* materia);

    void calcularHoras();
};

void Correlativa::addCorrelativa(Simple *materia) {
    matCorrelativas.push_back(materia);
}

void Correlativa::calcularHoras() {
    horas = 0.0;
    for (auto m : matCorrelativas) {
        horas += m->getHoras();
    }
}

class Compuesto : public Curso{
private:
    vector <Simple*> cursoSimple;
public:
    Compuesto (string cod, string n, string t) : Curso (cod, 0.0, n, t) {}

    void addCurso (Simple* c);
    void calcularHoras();
};

void Compuesto::addCurso(Simple *c) {
    cursoSimple.push_back(c);
}

void Compuesto::calcularHoras() {
    horas = 0.0;
    for (auto c : cursoSimple) {
        horas += c->getHoras();
    }
}

class Gestor {
private:
    vector <Curso*> cursos;
public:
    void addCurso (Curso* c);

    void crearTxt ();
    void crearB ();

    vector <Curso*> top5 ();
    map <string, int> temaMasFrecuente ();
};


void Gestor::addCurso(Curso *c) {
    cursos.push_back(c);
}

void Gestor::crearTxt () {
    ofstream archi ("nombre.txt");

    for (auto c : cursos) {
        archi << c->getNombre() << "   " << c->getHoras() << endl;
    }

    archi.close ();
}

void Gestor::crearB () {
    ofstream archi ("nombre.bnr", ios::binary);

    for (auto c : cursos) {
        // PARA STRING
        size_t len = c->getNombre().size();            // longitud de la descripción
        archi.write(reinterpret_cast<const char*>(&len), sizeof(len));  // guardo tamaño
        archi.write(c->getNombre().c_str(), len);     // guardo caracteres

        double precio = c->getHoras(); // guardamos en variable temporal
        archi.write(reinterpret_cast<char*>(&precio), sizeof(precio));
    }

    archi.close ();
}

vector <Curso*> Gestor::top5() {
    vector <Curso*> aux = cursos;

    sort (aux.begin(), aux.end(), [](Curso* a, Curso* b){
       return a->getHoras() < b->getHoras();
    });

    if (aux.size() > 5) {
        aux.resize(5); //me quedo solo con los primeros 5
    }

    return aux;
}

map <string, int> Gestor::temaMasFrecuente () {
    map <string, int> aux;

    for (auto c : cursos) {
        aux [c->getTema()]++;
    }

    auto it = max_element(aux.begin(), aux.end(),[](const pair<string,int>& a, const pair<string,int>& b){
        return a.second < b.second;
    }
    );

    map <string, int> resultado;
    if (it != aux.end()) {
        resultado [it->first] = it->second;
    }

    return resultado;
}

int main() {
    Gestor g;

    // ======= CURSOS SIMPLES =======
    Simple* s1 = new Simple("S1", 30, "Programacion C++", "Programacion");
    Simple* s2 = new Simple("S2", 20, "HTML y CSS", "Web");
    Simple* s3 = new Simple("S3", 25, "JavaScript", "Web");
    Simple* s4 = new Simple("S4", 15, "Git y GitHub", "Programacion");
    Simple* s5 = new Simple("S5", 40, "Bases de Datos", "Datos");
    Simple* s6 = new Simple("S6", 18, "Python Inicial", "Programación");

    g.addCurso(s1);
    g.addCurso(s2);
    g.addCurso(s3);
    g.addCurso(s4);
    g.addCurso(s5);
    g.addCurso(s6);

    // ======= CURSO CON CORRELATIVAS =======
    Correlativa* c1 = new Correlativa("C1", "Calculo 2", "Matematica");
    Simple* cal1 = new Simple("S7", 35, "Calculo 1", "Matematica");
    c1->addCorrelativa(cal1);
    c1->calcularHoras(); // suma horas de las correlativas
    g.addCurso(c1);

    // ======= CURSO COMPUESTO =======
    Compuesto* comp1 = new Compuesto("CMP1", "Programacion Web", "Web");
    comp1->addCurso(s2);
    comp1->addCurso(s3);
    comp1->calcularHoras(); // suma horas de las simples
    g.addCurso(comp1);

    // ======= CALCULAR HORAS DE TODOS LOS CURSOS =======
    for (Curso* c : g.top5()) {
        c->calcularHoras(); // recalcula horas de los 5 con menos horas
    }

    // ======= GUARDAR ARCHIVOS =======
    g.crearTxt();
    g.crearB();

    // ======= TOP 5 CON MENOS HORAS =======
    cout << "\nTop 5 cursos con menos horas:\n";
    auto top = g.top5();
    for (Curso* c : top) {
        cout << c->getNombre() << " - " << c->getHoras() << " hs\n";
    }

    // ======= TEMA MAS FRECUENTE =======
    auto temaMas = g.temaMasFrecuente();
    cout << "\nTema mas frecuente:\n";
    for (auto& par : temaMas) {
        cout << par.first << " -> " << par.second << " cursos\n";
    }

    // ======= LIBERAR MEMORIA =======
    delete s1; delete s2; delete s3; delete s4; delete s5; delete s6;
    delete cal1;
    delete c1;
    delete comp1;



    return 0;
}



