import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { WebSocketSubject, webSocket } from 'rxjs/webSocket';
import { Usuario, UsuarioRegister } from '../../interfaces/usuario';
import { Observable, filter, map } from 'rxjs';
import { environment } from '../../../environments/environment';

@Injectable({
  providedIn: 'root'
})
export class UsuarioService {
  private baseUrl = environment.BASE_URL;
  private wsUrl = environment.WS_URL;

  private socket$: WebSocketSubject<any> = webSocket(this.wsUrl);

  constructor(private http: HttpClient) { }

  getUsuarioByEmail(email: string): Observable<Usuario> {
    return this.http.get<Usuario>(`${this.baseUrl}/usuario/${email}`, {
      headers: {
        Authorization: `Bearer ${sessionStorage.getItem('access_token')}`
      }
    });
  }

  registerUsuario(usuario: UsuarioRegister): Observable<Usuario> {
    return this.http.post<Usuario>(`${this.baseUrl}/usuario`, usuario);
  }

  updateUsuarioByEmail(usuario: UsuarioRegister): Observable<Usuario> {
    const email = usuario.email;
    return this.http.put<Usuario>(`${this.baseUrl}/usuario/${email}`, usuario, {
      headers: {
        Authorization: `Bearer ${sessionStorage.getItem('access_token')}`
      }
    });
  }

  updateUsuarioEstadoInvitadoById(usuario: Usuario): Observable<any> {
    return this.http.put<any>(`${this.baseUrl}/usuario/${usuario.idusuario}/estado_invitado?estado_invitado=${usuario.estado_invitado}`, {}, {
      headers: {
        Authorization: `Bearer ${sessionStorage.getItem('access_token')}`
      }
    });
  }

  getUsarios(): Observable<Usuario[]> {
    return this.http.get<Usuario[]>(`${this.baseUrl}/usuarios`, {
      headers: {
        Authorization: `Bearer ${sessionStorage.getItem('access_token')}`
      }
    });
  }

  deleteUserById(id: number): Observable<Usuario> {
    return this.http.delete<Usuario>(`${this.baseUrl}/usuario/${id}`, {
      headers: {
        Authorization: `Bearer ${sessionStorage.getItem('access_token')}`
      }
    });
  }

  /**
   * The function `getUsuariosAutoregistrados` makes an HTTP GET request to retrieve a list of
   * autoregistered users, using an access token for authorization.
   * @returns an Observable of type Usuario[].
   */
  getUsuariosAutoregistrados(): Observable<Usuario[]> {
    return this.http.get<Usuario[]>(`${this.baseUrl}/usuarios/autoregistrados`, {
      headers: {
        Authorization: `Bearer ${sessionStorage.getItem('access_token')}`
      }
    });
  }


  /**
   * The function `getUsuariosAutoregistradosCountWs` returns an Observable that emits the count of
   * autoregistered users received from a WebSocket connection.
   * @returns an Observable of type number.
   */
  getUsuariosAutoregistradosCountWs(): Observable<number> {
    this.socket$ = webSocket(this.wsUrl + '/usuarios/autoregistrados/count');
    return this.socket$.asObservable().pipe(
      // Filtra los mensajes para obtener el conteo específico
      filter((message: any) => message.autoregistrados_count !== undefined),
      // Mapea el mensaje para obtener el conteo
      map((message: any) => message.autoregistrados_count)
    );
  }

  updateRolUsuarioById(id: number, rol: string): Observable<any> {
    return this.http.put<any>(`${this.baseUrl}/usuario/${id}/rol?rol=${rol}`, {}, {
      headers: {
        Authorization: `Bearer ${sessionStorage.getItem('access_token')}`
      }
    });
  }

  invitarUsuario(email: string, rol: string): Observable<Usuario> {
    return this.http.post<Usuario>(`${this.baseUrl}/usuario/invitar`, {
      email: email,
      rol: rol
    }, {
      headers: {
        Authorization: `Bearer ${sessionStorage.getItem('access_token')}`
      }
    });
  }
}
