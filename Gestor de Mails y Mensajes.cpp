#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <cstring>

using namespace std;

struct correo {
    int nro;
    char email [100];
    char texto [500];
};

struct mensaje {
    int nro;
    int nro_telefono;
    char texto [150];
};

class Mail {
private:
    correo datos;
public:
    Mail (correo d);

    correo getDatos () {return datos;}
    friend std::ostream& operator << (std::ostream& os, Mail& p);
};

Mail::Mail (correo d) {
    datos.nro = d.nro;
    strcpy (datos.email, d.email);
    strcpy (datos.texto, d.texto);
}

std::ostream& operator << (std::ostream& os, Mail& p) {
    os << "E " << p.getDatos().email << " " << p.getDatos().texto << endl;

    return os;
}

class Texto {
private:
    mensaje texto;
public:
    Texto (mensaje d);

    mensaje getTexto () {return texto;}
    friend std::ostream& operator << (std::ostream& os, Texto& p);

};


Texto::Texto (mensaje d) {
    texto.nro = d.nro;
    texto.nro_telefono = d.nro_telefono;
    strcpy (texto.texto, d.texto);
}

std::ostream& operator << (std::ostream& os, Texto& p) {
    os << "T " << p.getTexto().nro_telefono << " " << p.getTexto().texto << endl;

    return os;
}

class Gestor {
private:
    vector <Mail*> mails;
    vector <Texto*> msj;
public:
    void leerMail (const char* RutaArchivo);
    void leerMsj (const char* RutaArchivo);

    void crearTxt ();

    map<string,int> mailRepetidos();
    map<int,int> numRepetido();
};


void Gestor::leerMail(const char* RutaArchivo) {
    ifstream archivo(RutaArchivo, ios::binary);
    correo auxCorreo;

    while (archivo.read(reinterpret_cast<char*>(&auxCorreo), sizeof(correo))) {
        Mail* mail = nullptr;
        mail = new Mail(auxCorreo);

        if (mail) mails.push_back(mail);
    }

    archivo.close();
}

void Gestor::leerMsj(const char* RutaArchivo) {
    ifstream archivo(RutaArchivo, ios::binary);
    mensaje auxTexto;

    while (archivo.read(reinterpret_cast<char*>(&auxTexto), sizeof(mensaje))) {
        Texto* texto = nullptr;
        texto = new Texto (auxTexto);

        if (texto) msj.push_back(texto);
    }

    archivo.close();
}


void Gestor::crearTxt() {

    // ordenar mails por nro
    sort(mails.begin(), mails.end(), [](Mail* a, Mail* b) {
        return a->getDatos().nro < b->getDatos().nro;
    });

    // ordenar mensajes por nro
    sort(msj.begin(), msj.end(),[](Texto* a, Texto* b) {
        return a->getTexto().nro < b->getTexto().nro;
    });

    ofstream archi ("notificaciones.txt");

    for (auto* m : mails) {
        archi << *m << "\n";
    }

    for (auto* t : msj) {
        archi << *t << "\n";
    }

    archi.close();
}

map<string,int> Gestor:: mailRepetidos() {
    map<string,int> contador;

    // 1. Contar
    for (auto m : mails) {
        contador[m->getDatos().email]++;
    }

    return contador;
}

map<int,int> Gestor:: numRepetido() {
    map<int,int> contador;

    // 1. Contar
    for (auto t : msj) {
        contador[t->getTexto().nro_telefono]++;
    }

    return contador;
}





