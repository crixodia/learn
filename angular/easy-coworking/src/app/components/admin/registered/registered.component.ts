import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { Usuario } from '../../../interfaces/usuario';
import { UsuarioService } from '../../../services/usuario/usuario.service';
import { ToastService } from '../../../services/toast/toast.service';
import { TOAST_TYPE } from '../../../shared/components/toast/toast.component';

@Component({
  selector: 'app-registered',
  standalone: true,
  providers: [UsuarioService],
  imports: [CommonModule],
  templateUrl: './registered.component.html',
  styleUrl: './registered.component.css'
})
export class RegisteredComponent implements OnInit {
  users: Usuario[] = [];

  constructor(private usuarioService: UsuarioService, private toastService: ToastService) { }

  ngOnInit() {
    this.getUsers();
  }

  getUsers(): void {
    this.usuarioService.getUsuariosAutoregistrados().subscribe({
      next: (usuarios: Usuario[]) => {
        this.users = usuarios;
      },
      error: (error: any) => {
        this.toastService.showToast('Error', 'No se pudieron obtener los usuarios', TOAST_TYPE.error)
      }
    })
  }

  aceptarSolicitud(usuario: Usuario): void {
    usuario.estado_invitado = "aceptado";
    this.usuarioService.updateUsuarioEstadoInvitadoById(usuario).subscribe({
      next: () => {
        this.getUsers();
        this.toastService.showToast('Solicitud aceptada', 'La solicitud de ' + usuario.nombre + ' ha sido aceptada', TOAST_TYPE.success)
      },
      error: (error: any) => {
        if (error.status == 404) {
          this.toastService.showToast('Error', 'El usuario no existe.', TOAST_TYPE.error)
        }
        this.toastService.showToast('Error', 'No se pudo aceptar la solicitud de ' + usuario.nombre, TOAST_TYPE.error)
      }
    });
  }

  deleteUser(id: number): void {
    if (confirm("¿Está seguro de que desea rechazar y eliminar este usuario?")) {
      this.usuarioService.deleteUserById(id).subscribe({
        next: (response: any) => {
          this.getUsers();
        },
        error: (error: any) => {
          console.log(error);
        }
      });
    }
  }
}
