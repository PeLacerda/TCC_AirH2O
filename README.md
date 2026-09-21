# API
Treinamento de uma api teste,delivery

# COMANDOS DE DEPENDECIAS

# Instalar o ambiente virtual:
```
 python -m venv venv / venv\Scripts\activate
```
# Instalar as bibliotecas utilizadas
```
pip install fastapi uvicorn sqlalchemy passlib[bcrypt] python-jose[crypt] python-dotenv python-multipart
```
# Instalar parametros de utilidades do mysqlalchemy
```
pip install sqlalchemy-utils
```
# Biblioteca de migração de banco de dados
```
pip install alembic
```
# Commit do banco de dados
```
alembic revision --autogenerate -m ""
```
# Para rodar/enviar as informações para o banco de dados
```
alembic upgrade head
```