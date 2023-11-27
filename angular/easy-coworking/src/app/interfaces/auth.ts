export interface AuthInput {
    email: "string",
    contrasena: "string"
}

export interface AuthOutput {
    message: "string",
    access_token: "string",
}

export interface RolEstadoInvitado{
    rol: "string",
    estado_invitado: "string"
}