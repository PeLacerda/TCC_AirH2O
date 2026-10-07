#* para rodar o porjeto utilize o comando python -m uvicorn main:app --reload *#
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from passlib.context import CryptContext
from dotenv import load_dotenv
import os

load_dotenv()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

SECRET_KEY = os.getenv("SECRET_KEY")

app= FastAPI() 

bcrypt_context = CryptContext(schemes=["bcrypt"], deprecated="auto")

# importar os roteadores de requisições
from auth_routes import auth_router
from order_routes import order_router
from medicao_routes import medicao_router

# incluir os roteadores a aba main
app.include_router(auth_router)
app.include_router(order_router)
app.include_router(medicao_router)