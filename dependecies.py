from models import db
from sqlalchemy.orm import sessionmaker

def pegar_sessao():
    try:
        # criando conexão com o banco de dados
        Session=sessionmaker(bind=db)
        # criação da sessao para executar tal sessão,a session permitirá com que o banco de dados na+ão fique com requisições em aberto e faça outra busca em simultaneo
        session=Session()
        # yield retorna o valor mas nao encerra a sessao
        yield session
    finally:
        session.close()