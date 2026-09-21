from fastapi import APIRouter

order_router = APIRouter(prefix="/order",tags=["ordens"])

# criacao da rota "@order_router.get("/nome")"
@order_router.get("/")
async def saudacao():
    return{"Mensagem":"Ola seja bem vindo"}