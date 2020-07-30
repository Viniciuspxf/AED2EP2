#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
using namespace std;

bool ehSimbolo(char caracter) {

    switch (caracter) {
        case '*':
            return true;
            break;
        case '?':
            return true;
            break;
        case '"':
            return true;
            break;
        case ';':
            return true;
            break;
        case ':':
            return true;
            break;
        case '{':
            return true;
            break;
        case '}':
            return true;
            break;
        case '[':
            return true;
            break;
        case ']':
            return true;
            break;
        case '-':
            return true;
            break;
        case '.':
            return true;
            break;
        case ',':
            return true;
            break;
        case '!':
            return true;
            break;
        case '(':
            return true;
            break;
        case ')':
            return true;
            break;
        case '\357':
            return true;
            break;
        case '\273':
            return true;
            break;
        case '\277':
            return true;
            break;
    }

    return false;
}

class no {
    public:
        string palavra;
        vector<int> listaAdj;
};

class Grafo{
    private:
        int maiorComponente;
        int menorComponente;
        int minLetras;
        int nArestas;
        bool *visitado;
        int *dista;
        vector<no> nos;

        bool conectados(string a, string b) {
            int contador = 0, i, j, k, posicoes[3];
            bool conectado = false;
            string aux;
            if (a.size() == b.size()) {
                for (i = 0; i < a.size() && contador != 3; i++) {
                    if (a[i] != b[i]) {
                        posicoes[contador] = i;
                        contador++;
                    }
                }
    
                if (contador == 1) conectado = true;
                if (contador == 2 && a[posicoes[0]] == b[posicoes[1]] && a[posicoes[1]] == b[posicoes[0]])
                    conectado = true;
            }
            else {
                if (a.size() < b.size()) {
                    aux = a;
                    a = b;
                    b = aux;
                }

                if (b.size() == a.size() - 1) {
                    for (i = 0; i < a.size(); i++) {
                        contador = 0;
                        for (j = 0, k = 0; j < b.size(); j++, k++) {
                            if (k != i) contador += (a[k] != b[j]); 
                            else j--;
                        }
                        if (contador == 0) {
                            conectado = true;
                            break;
                        }
                    }
                }
            }
            return conectado;
        }
        
    public:

    Grafo(int k){
        fstream arquivo;
        string nomeArquivo, entrada;
        int diferenca, i;
        cout << "Digite o nome do arquivo: ";
        cin >> nomeArquivo;
        nArestas = 0;
        minLetras = k;
        arquivo.open(nomeArquivo);

        while (arquivo >> entrada) {
            for (i = 0; i < entrada.size() && ehSimbolo(entrada[i]); i++);
            
            entrada.erase(0, i);
            
            for (i = entrada.size(); i > 0 && ehSimbolo(entrada[i - 1]); i--);

            if (i != entrada.size())
                entrada.erase(i, entrada.size() - i);

            diferenca = insere(entrada);

            nArestas += (diferenca > 0 ? diferenca : 0);
        }
    }

    int bfsComponentes(int comeco) {
        int atual, tamComp = 0, j;
        queue<int> fila;
        fila.push(comeco);
        visitado[comeco] = true;
    
        while(!fila.empty()) {
            tamComp++;
            atual = fila.front();
            fila.pop();

            for (j = 0; j < nos[atual].listaAdj.size(); j++) 
                if (!visitado[nos[atual].listaAdj[j]]) {
                    visitado[nos[atual].listaAdj[j]] = true;
                    fila.push(nos[atual].listaAdj[j]);
                }
        }
        return tamComp;
    }

    void bfsDistancia(int comeco) {
        int i, distancia, atual;
        queue<int> fila;
        fila.push(comeco);

        for (i = 0; i < nos.size(); dista[i] = -1, i++);
        
        dista[comeco] = 0;

        while (!fila.empty()) {
            atual = fila.front();
            fila.pop();
                
            for (i = 0; i < nos[atual].listaAdj.size(); i++) {
                distancia = dista[nos[atual].listaAdj[i]];
                if (distancia == -1 || distancia > dista[atual] + 1) {
                    dista[nos[atual].listaAdj[i]] = dista[atual] + 1;
                    fila.push(nos[atual].listaAdj[i]);
                }
            }  
        }
    }

    int insere(string palavra){
        int i, j, contador = 0;
        bool achou = false;
        
        if (palavra.size() < minLetras)
            return -1;
        
        for (i = 0; i < nos.size() - achou; i++) {
            
            if (nos[i].palavra == palavra)
                return -1;

            if (conectados(nos[i].palavra, palavra)) {
                for (j = 0; j < nos[i].listaAdj.size() && !achou; j++) {
                    if (nos[nos[i].listaAdj[j]].palavra == palavra)
                        return -1;     
                }
                contador++;
                if (!achou) {
                    nos.push_back({palavra, {}});
                    achou = true;
                }
                
                nos[nos.size() - 1].listaAdj.push_back(i);
                nos[i].listaAdj.push_back(nos.size() - 1);
            }
        }

        if (!achou)
            nos.push_back({palavra, {}});
        return contador;
    }

    int vertices(){
        return nos.size();
    }

    int arestas(){
        return nArestas;
    }

    double grauMedio() {
        return ((nos.size() >= 1) ? ((double) 2*nArestas)/nos.size(): 0);
    }

    double densidade() {
        return ((nos.size() > 1) ? ((double) 2*nArestas)/(nos.size()*(nos.size() - 1)) : 0);
    }

    int componentes(){
        int nComp = 0, i, tamCompAtual;
        visitado = new bool[nos.size()];
        
        maiorComponente = 0;
        menorComponente = 0;

        for (i = 0; i < nos.size(); visitado[i] = false, i++); 
        
        for (i = 0; i < nos.size(); i++) {
            if (!visitado[i]) {
                nComp++;
                tamCompAtual = bfsComponentes(i);
                maiorComponente = (maiorComponente < tamCompAtual ? tamCompAtual : maiorComponente);
                menorComponente = (menorComponente == 0 || menorComponente > tamCompAtual ? tamCompAtual : menorComponente);
            }
        }
        delete [] visitado;
        return nComp;
    }

    double tamMedComponentes() {
        return (nos.size() != 0 ? ((double) nos.size())/componentes() : 0);
    }

    void maxMinComponentes() {
        componentes();
        cout << "A maior componente tem " << maiorComponente << " vértices." << endl;
        cout << "A menor componente tem " << menorComponente << " vértices." << endl;
    }

    bool conexo(){
        return componentes() <= 1;
    }

    int tamComp(string palavra){
        int i, tam = 0, comeco;
        
        for (i = 0; i < nos.size() && nos[i].palavra != palavra; i++);

        if (i == nos.size()) return -1;

        comeco = i;

        visitado = new bool[nos.size()];

        for (i = 0; i < nos.size(); visitado[i] = false, i++);

        tam = bfsComponentes(comeco);
        delete [] visitado;
        return tam;
    }

    int dist(string a, string b){
        int i, inicio = -1, fim = -1, distancia;
        
        for (i = 0; i < nos.size(); i++) {
            if (nos[i].palavra == a) inicio = i;
            if (nos[i].palavra == b) fim = i;
        }

        if (inicio == -1 || fim == -1) return -1;
        dista = new int[nos.size()];
        bfsDistancia(inicio);

        distancia = dista[fim];
        delete [] dista;
        return distancia;
    }

    double mediaDist() {
        int i, j, somatoria = 0, total = 0;
        dista = new int[nos.size()];
        queue<int> fila;

        for (j = 0; j < nos.size(); j++) {
            
            bfsDistancia(j);
            
            for (i = 0; i < nos.size(); i++) {
                if (j != i && dista[i] != -1) {
                    somatoria += dista[i];
                    total++;
                }
            }
        }
        delete [] dista;
        return (total != 0 ? ((double) somatoria)/total : 0);
    }

    bool emCiclo(string a) {
        queue<int> fila;
        int i, comeco, atual, j;
        bool *marcado;

        for (i = 0; i < nos.size() && nos[i].palavra != a; i++);
        
        if (i == nos.size()) return false;

        comeco = i;
        marcado = new bool[nos.size()];
        visitado = new bool[nos.size()];

        for (i = 0; i < nos.size(); i++)
            marcado[i] = false;
        
        for (i = 0; i < nos[comeco].listaAdj.size(); i++)
            marcado[nos[comeco].listaAdj[i]] = true;
        
        for (j = 0; j < nos[comeco].listaAdj.size(); j++) {

            for (i = 0; i < nos.size(); visitado[i] = false, i++);
            
            visitado[comeco] = true;
            visitado[nos[comeco].listaAdj[j]] = true;

            fila.push(nos[comeco].listaAdj[j]);

            while (!fila.empty()) {
                atual = fila.front();

                fila.pop();

                for (i = 0; i < nos[atual].listaAdj.size(); i++) {
                    if (!visitado[nos[atual].listaAdj[i]]) {
                        if (marcado[nos[atual].listaAdj[i]]) {
                            delete [] marcado;
                            delete [] visitado;
                            return true;
                        }
                        else {
                            visitado[nos[atual].listaAdj[i]] = true;
                            fila.push(nos[atual].listaAdj[i]);
                        }
                    }
                } 
            }
        
        }
        delete [] marcado;
        delete [] visitado;
        return false;
    }

    bool emCiclo(string a, string b) {
        int i, comeco = -1, fim = -1, atual;
        queue<int> fila;

       for (i = 0; i < nos.size(); i++) {
            if (nos[i].palavra == a) comeco = i;
            if (nos[i].palavra == b) fim = i;
        }

        if (comeco == -1 || fim == -1) return false;

        visitado = new bool[nos.size()];
        dista = new int[nos.size()];
        
        bfsDistancia(comeco);
        
        if (dista[fim] == -1) return false;

        for (i = 0; i < nos.size(); visitado[i] = false, i++);

        atual = fim;

        while (atual != comeco) {
            for (i = 0; i < nos[atual].listaAdj.size(); i++) {
                if (dista[atual] - 1 == dista[nos[atual].listaAdj[i]]) {
                    atual = nos[atual].listaAdj[i];
                    visitado[atual] = true;
                    break;
                }
            }
        }

        fila.push(comeco);
        visitado[fim] = true;

        while (!fila.empty()) {
            atual = fila.front();

            fila.pop();

            for (i = 0; i < nos[atual].listaAdj.size(); i++) {

                if (atual != comeco && nos[atual].listaAdj[i] == fim) {
                    delete [] dista;
                    delete [] visitado;
                    return true;
                }

                if (!visitado[nos[atual].listaAdj[i]]) {
                    visitado[nos[atual].listaAdj[i]] = true;
                    fila.push(nos[atual].listaAdj[i]);
                }
            }
        }

        delete [] dista;
        delete [] visitado;
        return false;
    }

    void print() {
        int i, j;

        for (i = 0; i < nos.size(); i++) {
            cout << "palavra: " << nos[i].palavra << "\n\n";
            
            for (j = 0; j < nos[i].listaAdj.size(); j++) {
                cout << nos[nos[i].listaAdj[j]].palavra << endl;
            }
            cout << endl;
        }
    }

    void printDistancias() {
        int i, j;

        dista = new int[nos.size()];

        for (i = 0; i < nos.size(); i++) {
            
            bfsDistancia(i);

            for (j = 0; j < nos.size(); j++)
                cout << "distancia(" << nos[i].palavra << "," << nos[j].palavra << ") = " << dista[j] << endl;
            cout << endl;
        }
        delete [] dista;
    }

    void printCiclos() {
        int i, j;

        for (i = 0; i < nos.size(); i++) {
            if (emCiclo(nos[i].palavra)) 
                cout << "'" << nos[i].palavra << "' está em ciclo." << endl;
        }
        cout << endl;
        for (i = 0; i < nos.size(); i++) {
            for (j = i + 1; j < nos.size(); j++) {
                if (emCiclo(nos[i].palavra, nos[j].palavra))
                cout << "'" << nos[i].palavra << "' e '" << nos[j].palavra << "' estão em ciclo." << endl;
             }
        }
    }

};
