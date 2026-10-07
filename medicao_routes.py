from fastapi import APIRouter, Depends, HTTPException
from pydantic import BaseModel
from typing import List
from models import Medicao
from dependecies import pegar_sessao

medicao_router = APIRouter(prefix="/api/medicoes", tags=["Medicoes"])


# JSON enviado pelo ESP32 (airh2o_monitor.ino)
class MedicaoEntrada(BaseModel):
    dispositivo_id: str
    dispositivo_nome: str | None = None
    nivel_percentual: int
    nivel_litros_estimado: float | None = None
    capacidade_litros: float | None = None
    sensores_ativos: List[int] = [0, 0, 0, 0]
    timestamp_unix: int | None = None


# ESP32 -> API
@medicao_router.post("", status_code=201)
async def receber_medicao(dados: MedicaoEntrada, session=Depends(pegar_sessao)):
    s25, s50, s75, s100 = [bool(x) for x in dados.sensores_ativos[:4]]
    medicao = Medicao(
        dispositivo_id=dados.dispositivo_id,
        dispositivo_nome=dados.dispositivo_nome,
        nivel_percentual=dados.nivel_percentual,
        nivel_litros=dados.nivel_litros_estimado,
        capacidade_litros=dados.capacidade_litros,
        sensor25=s25, sensor50=s50, sensor75=s75, sensor100=s100,
    )
    session.add(medicao)
    session.commit()
    return {"Mensagem": "medicao registrada"}


# Front -> API (aba do nivel do tonel)
@medicao_router.get("/ultima")
async def ultima_medicao(session=Depends(pegar_sessao)):
    m = session.query(Medicao).order_by(Medicao.id.desc()).first()
    if not m:
        raise HTTPException(status_code=404, detail="Nenhuma medicao encontrada")
    return {
        "nivel_percentual": m.nivel_percentual,
        "nivel_litros": m.nivel_litros,
        "capacidade_litros": m.capacidade_litros,
        "criado_em": m.criado_em.isoformat() + "Z",
    }
