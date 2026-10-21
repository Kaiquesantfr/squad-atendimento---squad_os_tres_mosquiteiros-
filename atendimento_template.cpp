#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

// ================================================================
// ESTRUTURA BASE - No generico
// ================================================================
struct No {
    string nome;
    int    urgencia;   // 1=baixa, 2=media, 3=alta (usado apenas por Paciente)
    No*    prox;
    No(const string& n, int u = 0) : nome(n), urgencia(u), prox(nullptr) {}
};

// ================================================================
// R1 - FILA DE PACIENTES (estrutura FIFO)
// ================================================================
struct FilaPacientes {
    No* frente = nullptr;
    No* tras   = nullptr;

    // TODO 1: Implemente enqueue(nome, urgencia)
    void enqueue(const string& nome, int urgencia) {
        No* novo = new No(nome, urgencia);
        if (isEmpty()) {
            frente = tras = novo;
        } else {
            tras->prox = novo;
            tras = novo;
        }
    }

    // TODO 2: Implemente dequeue()
    string dequeue() {
        if (isEmpty()) return "";
        No* temp = frente;
        string nome = temp->nome;
        frente = frente->prox;
        if (frente == nullptr) tras = nullptr;
        delete temp;
        return nome;
    }

    // TODO 3: Implemente frente_peek()
    string frente_peek() const {
        if (isEmpty()) return "";
        return frente->nome;
    }

    bool isEmpty() const { return frente == nullptr; }

    int tamanho() const {
        int count = 0;
        No* temp = frente;
        while (temp != nullptr) {
            count++;
            temp = temp->prox;
        }
        return count;
    }

    void imprimirFila() const {
        if (isEmpty()) {
            cout << "(fila vazia)\n";
            return;
        }
        No* temp = frente;
        int posicao = 1;
        while (temp != nullptr) {
            cout << "  " << posicao++ << ". " << temp->nome 
                 << " (Urgencia: " << temp->urgencia << ")\n";
            temp = temp->prox;
        }
    }

    ~FilaPacientes() {
        while (!isEmpty()) dequeue();
    }
};

bool perguntoSimNao(const string& mensagem) {
    string resposta;
    while (true) {
        cout << mensagem;
        cin >> resposta;

        string respLower = resposta;
        for (char &c : respLower) c = tolower(c);

        if (respLower == "s" || respLower == "sim") return true;
        if (respLower == "n" || respLower == "nao") return false;

        cout << "Resposta invalida! Digite 's' para sim ou 'n' para nao.\n";
    }
}

int main() {
    FilaPacientes fila;

    cout << "  Sistema de Triagem e Atendimento Hospitalar      \n";

    // -- R1: CADASTRO DE PACIENTES ----------------------------
    cout << "=== CADASTRO DE PACIENTES ===\n";
    bool maisPacientes = true;
    do {
        string nome;
        int urgencia;

        cout << "Nome do paciente: ";
        cin >> ws; 
        getline(cin, nome);

        cout << "Nivel de urgencia (1=baixa, 2=media, 3=alta): ";
        cin >> urgencia;

        fila.enqueue(nome, urgencia);

        maisPacientes = perguntoSimNao("Tem mais pacientes? (s/n): ");
        cout << "---------------------------------------------------\n";
    } while (maisPacientes);

    cout << "\nPacientes aguardando na fila (" << fila.tamanho() << " no total):\n";
    fila.imprimirFila();

    return 0;
}