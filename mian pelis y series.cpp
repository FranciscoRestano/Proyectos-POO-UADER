#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct datosVideo {
    int numero;
    char nombre [200];
    char tipo;
};

struct datosVotacion {
    int numero;
    int votacion;
};

class Video {
protected:
    datosVideo* datos;
    vector<int> votaciones;
    double valoracion;
public:
    Video (datosVideo* d);
    void addVotacion (int v);
    int getNumero () {return datos->numero;}
    string getNombre () { return datos->nombre;}
    double getValoracion () {return valoracion;}
    int getCantVotaciones ();
    void calcularValoracion ();
};

Video::Video (datosVideo *d) {
    datos->numero = d->numero;
    datos->tipo = d->tipo;
    strcpy (datos->nombre, d->nombre);
    valoracion = 0;
}

void Video::addVotacion(int v) {
    votaciones.push_back(v);
}

void Video::calcularValoracion() {
    for (int i=0;i<votaciones.size();i++) {
        valoracion += votaciones[i];
    }
    valoracion /= votaciones.size();
}

int Video::getCantVotaciones() {
    int acum=0;
    for (auto v : votaciones) {
        acum++;
    }
    return acum;
}

class Gestor {
private:
    vector<Video*> seriesyPeliculas;
public:
    Gestor (const char* RutaArchivo);
    void leerBnb (const char* RutaArchivo);
    void crearTxt ();

    vector<double> top5 ();
    map<string, int> pelimasVotada ();
};

Gestor::Gestor(const char* RutaArchivo) {
    ifstream archivo(RutaArchivo, ios::binary);
    datosVideo datos;

    while (archivo.read(reinterpret_cast<char*>(&datos), sizeof(datosVideo))) {
        Video* video = new Video (datos);
        seriesyPeliculas.push_back(video);
    }

    archivo.close();
}

void Gestor::leerBnb(const char *RutaArchivo) {
    ifstream archivo(RutaArchivo, ios::binary);
    datosVotacion datos;

    while (archivo.read(reinterpret_cast<char*>(&datos), sizeof(datosVotacion))) {
        for (auto s : seriesyPeliculas) {
            if (s->getNumero() == datos.numero) {
                s->addVotacion(datos.votacion);
            }
        }
    }

    archivo.close();
}

void Gestor::crearTxt() {
    ofstream archi ("valoraciones.txt");

    for (auto s : seriesyPeliculas) {
        s->calcularValoracion();
        archi << s->getNombre() << " .... " << s->getValoracion() << "\n";
    }

    archi.close ();
}

vector<double> Gestor::top5() {
    vector <double> aux = seriesyPeliculas;

    sort(aux.begin(), aux.end(), [](Video* a, Video* b){
        return a->getValoracion() > b->getValoracion() // orden descendente
    });

    // Me quedo con los primeros 5 o menos si hay menos elementos
    if (aux.size() > 5) {
        aux.resize(5);
    }

    return aux;
}

map<string,int> Gestor::pelimasVotada() {
    map<string,int> contador;

    // 1. Contar
    for (auto c : seriesyPeliculas) {
        contador[c->getNombre()] = c->getCantVotaciones();
    }

    // 2. Buscar el mayor
    auto it = max_element(
        contador.begin(), contador.end(),
        [](const pair<string,int>& a, const pair<string,int>& b){
            return a.second < b.second;
        }
        );

    // 3. Crear un map con un solo elemento
    map<string,int> resultado;
    if (it != contador.end()) {
        resultado[it->first] = it->second;
    }

    // 4. Retornar
    return resultado;
}

map <string, int> Gestor::votos () {
    map<string,int> contador;

    for (auto s : seriesyPeliculas) {
        contador[s->getTipo()] += s->getCantVotaciones();
    }

    return contador;
}















