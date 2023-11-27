import { Component, OnInit, inject } from '@angular/core';
import { CommonModule } from '@angular/common';
import { Router, RouterModule, RouterOutlet } from '@angular/router';
import { HttpClientModule } from '@angular/common/http';
import { UsuarioService } from '../../services/usuario/usuario.service';
import { NavbarComponent } from './navbar/navbar.component';
import { FormBuilder, FormGroup, ReactiveFormsModule, Validators } from '@angular/forms';
import { TOAST_TYPE } from '../../shared/components/toast/toast.component';
import { ToastService } from '../../services/toast/toast.service';

@Component({
  selector: 'app-admin',
  standalone: true,
  providers: [UsuarioService],
  imports: [CommonModule, HttpClientModule, NavbarComponent, RouterOutlet, RouterModule, ReactiveFormsModule],
  templateUrl: './admin.component.html',
  styleUrl: './admin.component.css'
})
export class AdminComponent implements OnInit {
  autoregistrados: number = 0;
  isLoading: boolean = true;

  invitarForm: FormGroup;

  constructor(
    private router: Router,
    private usuarioService: UsuarioService,
    private fb: FormBuilder,
    private toastService: ToastService
  ) {
    this.invitarForm = this.fb.group({
      email: ['', [Validators.required, Validators.email]],
      rol: ['general', [Validators.required]]
    });
  }

  logout() {
    sessionStorage.clear();
    this.router.navigate(['/login']);
  }

  countAutoregistrados() {
    this.usuarioService.getUsuariosAutoregistradosCountWs().subscribe({
      next: (count: number) => {
        this.autoregistrados = count;
        sessionStorage.setItem('autoregistrados', count.toString());
      },
      error: (error: any) => {
        this.toastService.showToast('Error', error, TOAST_TYPE.error);
      }
    });
  }

  ngOnInit(): void {
    const router = this.router;
    const usuarioString = sessionStorage.getItem('usuario');

    if (usuarioString) {
      const usuario = JSON.parse(usuarioString);
      this.usuarioService.getUsuarioByEmail(usuario.email).subscribe({
        next: (data: any) => {
          if (data.estado_invitado === 'autoregistrado') {
            sessionStorage.clear();
            router.navigate(['login']);
          } else if (data.estado_invitado === 'invitado') {
            router.navigate(['complete-profile']);
          }
          if (data.rol != 'administrador') {
            router.navigate(['home']);
          }
          this.isLoading = false;
        },
        error: (error: any) => {
          this.toastService.showToast('Error', error, TOAST_TYPE.error);
          sessionStorage.clear();
          router.navigate(['login']);
        }
      });
      this.countAutoregistrados();
    }
  }

  invitarUsuario() {
    if (this.invitarForm.valid) {
      const usuario = this.invitarForm.value;
      this.usuarioService.invitarUsuario(usuario.email, usuario.rol).subscribe({
        next: () => {
          this.toastService.showToast('Invitación enviada', usuario.email + ' ha sido invitado a registrarse en la plataforma.', TOAST_TYPE.success);
          this.invitarForm.reset();
        },
        error: (error: any) => {
          if (error.status === 409) {
            this.toastService.showToast('Error', 'El usuario ya ha sido invitado o ya pertenece a la plataforma.', TOAST_TYPE.error);
          } else {
            this.toastService.showToast('Error', 'Ocurrió un error al enviar la invitación.', TOAST_TYPE.error);
          }
        }
      });
    }
  }
}
