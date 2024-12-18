from flask import Flask, request, jsonify
from langchain.document_loaders.csv_loader import CSVLoader
from langchain.text_splitter import RecursiveCharacterTextSplitter
from langchain.embeddings import HuggingFaceEmbeddings
from langchain.vectorstores import FAISS
from langchain.chains import ConversationalRetrievalChain
from langchain_ollama import OllamaLLM  # Importer OllamaLLM
import sys
from flask_cors import CORS

# Initialiser l'application Flask
app = Flask(__name__)
CORS(app)

DB_FAISS_PATH = "vectorstore/db_faiss"
CSV_FILE_PATH = "data/data.csv"

# Charger les données du fichier CSV
def load_data(file_path):
    loader = CSVLoader(file_path=file_path, encoding="utf-8", csv_args={'delimiter': ','})
    data = loader.load()
    return data

# Préparer les embeddings et la base de données FAISS
def prepare_faiss(data):
    # Diviser le texte en morceaux
    text_splitter = RecursiveCharacterTextSplitter(chunk_size=500, chunk_overlap=20)
    text_chunks = text_splitter.split_documents(data)

    # Utiliser HuggingFace pour créer des embeddings
    embeddings = HuggingFaceEmbeddings(model_name='sentence-transformers/all-MiniLM-L6-v2')
    
    # Créer une base de données FAISS à partir des embeddings
    docsearch = FAISS.from_documents(text_chunks, embeddings)
    docsearch.save_local(DB_FAISS_PATH)
    
    return docsearch

# Charger le modèle Llama2 via OllamaLLM
def load_llama_model():
    # Utiliser OllamaLLM pour charger le modèle Llama2
    model = OllamaLLM(model="llama2")
    return model

# Créer une instance de ConversationalRetrievalChain
def create_qa_chain(docsearch):
    model = load_llama_model()
    qa_chain = ConversationalRetrievalChain.from_llm(model, retriever=docsearch.as_retriever())
    return qa_chain

# Route pour gérer la requête de chat
@app.route("/chat", methods=["POST"])
def chat():
    # Récupérer la question de l'utilisateur
    user_input = request.json.get("user_input", "")
    
    if not user_input:
        return jsonify({"error": "No user input provided"}), 400

    # Charger les données CSV et créer la base de données FAISS
    data = load_data(CSV_FILE_PATH)
    docsearch = prepare_faiss(data)

    # Créer la chaîne de conversation
    qa_chain = create_qa_chain(docsearch)

    # Gérer l'historique de la conversation
    chat_history = []

    # Obtenir la réponse de l'IA via le modèle Llama2
    result = qa_chain({"question": user_input, "chat_history": chat_history})

    # Retourner la réponse au client
    return jsonify({"response": result['answer']})


if __name__ == "__main__":
    app.run(debug=True, port=5000)
