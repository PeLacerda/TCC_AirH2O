from sqlalchemy import create_engine, Column, String,Integer,ForeignKey,Boolean
from sqlalchemy.orm import declarative_base 

# conexao com o banco
db = create_engine("sqlite:///banco.db")

# criação da base do banco
Base = declarative_base()

# criar classes/tabelas do banco de dados
# nome
# email
# telefone
# senha
class Usuario(Base):
    #__tablename__= "nomeTabela" permite lterar o nome da tabela
    # nullable significa nao permitir o campo vazio

    __tablename__="usuarios"

    id = Column("id",Integer,primary_key=True,autoincrement=True)
    nome = Column("Nome",String)
    email = Column("Email",String,nullable=False)
    telefone = Column("Telefone", Integer)
    senha = Column("Senha",String)
    admin = Column("Admin",Boolean,default=False)
    endereco = Column("Endereco",String)

    # a funcao "__init__" sera executa toda as vezes que um novo usuario for criado,esta definicao obrigara a passa algumas tabelas do banco de dados para a criação de um novo usuario
    def __init__(self,nome,email,telefone,senha,endereco,admin=False):
        self.nome = nome
        self.email=email
        self.telefone=telefone
        self.senha = senha       
        self.endereco=endereco
        self.admin=admin