import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { UsuarioService } from '../../../services/usuario/usuario.service';
import { Usuario } from '../../../interfaces/usuario';
import { HttpClientModule } from '@angular/common/http';
import { FormBuilder, FormGroup, ReactiveFormsModule, Validators } from '@angular/forms';
import { ToastService } from '../../../services/toast/toast.service';
import { TOAST_TYPE } from '../../../shared/components/toast/toast.component';

@Component({
  selector: 'app-users',
  standalone: true,
  providers: [UsuarioService],
  imports: [CommonModule, HttpClientModule, ReactiveFormsModule],
  templateUrl: './users.component.html',
  styleUrl: './users.component.css'
})
export class UsersComponent implements OnInit {
  users: Usuario[] = [];
  selectedUser: Usuario = {} as Usuario;

  selectedUsuarioForm: FormGroup;

  constructor(private usuarioService: UsuarioService, private fb: FormBuilder, private toastService: ToastService) {
    this.selectedUsuarioForm = this.fb.group({
      nombre: [''],
      empresa: [''],
      direccion: [''],
      telefono: [''],
      email: [''],
      cargo: [''],
      rol: ['', [Validators.required]],
    });
  }

  ngOnInit() {
    this.loadUsers();
  }

  loadUsers() {
    this.usuarioService.getUsarios().subscribe(
      next => {
        this.users = next;
      }
    );
  }

  openModal(user: Usuario) {
    this.selectedUser = user;

    this.selectedUsuarioForm.patchValue({
      nombre: user.nombre,
      empresa: user.empresa,
      direccion: user.direccion,
      telefono: user.telefono,
      email: user.email,
      cargo: user.cargo,
      rol: user.rol,
    });
  }

  closeModal() {
    this.selectedUser = {} as Usuario;
    this.loadUsers();
  }

  deleteUser(id: number) {
    const currentUsuario = JSON.parse(sessionStorage.getItem('usuario') || 'null');

    this.usuarioService.getUsuarioByEmail(currentUsuario.email).subscribe({
      next: (autenticado: Usuario) => {
        if (autenticado.idusuario === id) {
          this.toastService.showToast('Error', 'No puede eliminar su propio usuario', TOAST_TYPE.error);
          this.closeModal();
        } else if (autenticado.rol === 'administrador') {
          if (confirm('¿Está seguro que desea eliminar este usuario?')) {
            this.usuarioService.deleteUserById(id).subscribe({
              next: () => {
                this.toastService.showToast('Usuario eliminado', this.selectedUser.nombre + ' ha sido eliminado', TOAST_TYPE.warning);
                this.closeModal();
              }, error: (error: any) => {
                this.toastService.showToast('Error', 'Ha ocurrido un error: ' + error, TOAST_TYPE.error);
              }
            });
          }
        }
      },
      error: (error: any) => {
        this.toastService.showToast('Error', 'Ha ocurrido un error: ' + error, TOAST_TYPE.error);
      }
    });
  }

  updateRol(user: Usuario) {
    const currentUsuario = JSON.parse(sessionStorage.getItem('usuario') || 'null');
    if (user.rol === this.selectedUsuarioForm.get('rol')?.value) {
      this.closeModal();
      return;
    }

    this.usuarioService.getUsuarioByEmail(currentUsuario.email).subscribe({
      next: (autenticado: Usuario) => {
        if (autenticado.idusuario === user.idusuario) {
          this.toastService.showToast('Error', 'No puede cambiar su propio rol', TOAST_TYPE.error);
          this.closeModal();
        } else if (autenticado.rol === 'administrador') {
          user.rol = this.selectedUsuarioForm.get('rol')?.value;
          this.usuarioService.updateRolUsuarioById(user.idusuario, user.rol).subscribe({
            next: () => {
              this.toastService.showToast('Rol actualizado', user.nombre + ' ahora es ' + user.rol, TOAST_TYPE.success);
              this.closeModal();
            }, error: (error: any) => {
              this.toastService.showToast('Error', 'Ha ocurrido un error: ' + error, TOAST_TYPE.error);
            }
          });
        }
      },
      error: (error: any) => {
        this.toastService.showToast('Error', 'Ha ocurrido un error: ' + error, TOAST_TYPE.error);
      }
    });
  }

}
