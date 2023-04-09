from jwt import encode, decode

# USE ENVIRONMENT VARIABLES TO STORE THE SECRET KEY
# THIS IS JUST AN EXAMPLE
def create_token(data: dict):
    return encode(payload=data, key='secret', algorithm='HS256')


def validate_token(token: str) -> dict:
    return decode(token, key="secret", algorithms=['HS256'])