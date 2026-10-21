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

// ================================================================
// R2 - HISTORICO COM PILHA (estrutura LIFO)
// ================================================================
struct PilhaHistorico {
    No* topo = nullptr;

    // TODO 4: Implemente push(nome)
    void push(const string& nome) {
        No* novo = new No(nome);
        novo->prox = topo;
        topo = novo;
    }

    // TODO 5: Implemente pop()
    string pop() {
        if (isEmpty()) return "";
        No* temp = topo;
        string nome = temp->nome;
        topo = topo->prox;
        delete temp;
        return nome;
    }

    // TODO 6: Implemente peek()
    string peek() const {
        if (isEmpty()) return "";
        return topo->nome;
    }

    bool isEmpty() const { return topo == nullptr; }

    ~PilhaHistorico() {
        while (!isEmpty()) pop();
    }
};

// ================================================================
// R3 - LISTA DE MEDICOS DISPONIVEIS (lista encadeada)
// ================================================================
struct ListaMedicos {
    No* head = nullptr;

    // TODO 7: Implemente adicionarMedico(nome)
    void adicionarMedico(const string& nome) {
        No* novo = new No(nome);
        if (head == nullptr) {
            head = novo;
        } else {
            No* temp = head;
            while (temp->prox != nullptr) {
                temp = temp->prox;
            }
            temp->prox = novo;
        }
    }

    // TODO 8: Implemente chamarMedico()
    string chamarMedico() {
        if (head == nullptr) return "";
        No* temp = head;
        string nome = temp->nome;
        head = head->prox;
        delete temp;
        return nome;
    }

    // TODO 9: Implemente percorrer()
    void percorrer(const string& prefixo = "") const {
        if (!prefixo.empty()) cout << prefixo << ": ";
        
        if (head == nullptr) {
            cout << "(lista vazia)\n";
            return;
        }
        
        No* temp = head;
        while (temp != nullptr) {
            cout << temp->nome;
            if (temp->prox != nullptr) cout << ", ";
            temp = temp->prox;
        }
        cout << "\n";
    }

    bool temMedico() const { return head != nullptr; }

    ~ListaMedicos() {
        while (head) {
            No* t = head;
            head  = head->prox;
            delete t;
        }
    }
};

// ================================================================
// R4 - RELATORIO E HISTORICO COMPLETO
// ================================================================

// TODO 10: Implemente a funcao imprimirHistorico(PilhaHistorico&)
void imprimirHistorico(PilhaHistorico& hist) {
    cout << "\n=== Relatorio de Atendimentos ===\n";
    if (hist.isEmpty()) {
        cout << "(nenhum atendimento registrado)\n";
        return;
    }
    
    PilhaHistorico aux;
    
    while (!hist.isEmpty()) {
        string atendimento = hist.pop();
        cout << "  - " << atendimento << "\n";
        aux.push(atendimento);
    }
    
    while (!aux.isEmpty()) {
        hist.push(aux.pop());
    }
}

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
    FilaPacientes  fila;
    PilhaHistorico historico;
    ListaMedicos   medicos;

    cout << "  Sistema de Triagem e Atendimento Hospitalar      \n";

    cout << "=== CADASTRO DE MEDICOS ===\n";
    bool maisMedicos = true;
    do {
        string nome;
        cout << "Nome do medico: ";
        cin >> ws;
        getline(cin, nome);

        medicos.adicionarMedico(nome);

        maisMedicos = perguntoSimNao("Tem mais medicos? (s/n): ");
        cout << "---------------------------------------------------\n";
    } while (maisMedicos);

    cout << "\nMedicos cadastrados no plantao:\n";
    medicos.percorrer("Medicos");

    // -- R1: CADASTRO DE PACIENTES ----------------------------
    cout << "\n=== CADASTRO DE PACIENTES ===\n";
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

 
    cout << "\n=== CONSULTAS EM ANDAMENTO ===\n";
    int atendimentoNum = 1;

    while (!fila.isEmpty() && medicos.temMedico()) {
        string paciente = fila.dequeue();
        string medico   = medicos.chamarMedico();

        cout << "  [" << atendimentoNum++ << "] " << paciente 
             << " vai consultar com " << medico << "\n";

        historico.push(medico + " -> " + paciente);
        medicos.adicionarMedico(medico);
    }


    imprimirHistorico(historico);

    return 0;
}

