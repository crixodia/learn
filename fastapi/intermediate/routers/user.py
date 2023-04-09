from fastapi import APIRouter
from fastapi.responses import JSONResponse
from utils.jwt_manager import create_token
from schemas.user import User


user_router = APIRouter()


@user_router.post('/login', tags=['auth'])
def login(user: User):
    if user.password == '1234' and user.email == 'admin@gmail.com':
        token: str = create_token({'email': user.email})
        return JSONResponse(content={"token": token}, status_code=200)
