export interface Usuario {
    empresa: string,
    nombre: string,
    direccion: string,
    telefono: string,
    email: string,
    estado_invitado: string,
    rol: string,
    cargo: string,
    idusuario: number,
}

export interface UsuarioRegister {
    empresa: string,
    nombre: string,
    direccion: string,
    telefono: string,
    email: string,
    estado_invitado: string,
    rol: string,
    cargo: string,
    contrasena: string
}

