from fastapi import APIRouter,Depends
from models import Usuario
from dependecies import pegar_sessao
from main import bcrypt_context

auth_router = APIRouter(prefix= "/auth",tags=["Autenticacao"])

@auth_router.get("/")

async def home():
    # para deixar comentario usar 6 aspas
    """
    Esta rota é de autenticacao, somente usuarios autorizados conseguem o acesso
    """

    return{"Mensagem":"Autenticado com sucesso","Autenticado":False}

@auth_router.post("/criar_usuario")
async def criar_conta(email:str,senha:str, nome:str,telefone:int,endereco:str,session=Depends(pegar_sessao)):
    usuario = session.query(Usuario).filter(Usuario.email== email).first()

    if usuario:
        return{"Mensagem":"Existe um usuario ja com este email"}
    else:
        senha_criptografada = bcrypt_context.hash(senha)
        novo_usuario = Usuario(nome,email,telefone,senha_criptografada,endereco)
        session.add(novo_usuario)
        session.commit()
        return{"Mensagem":"usuario cadastrado com sucesso"}