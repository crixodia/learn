import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { Router } from '@angular/router';
import { HttpClientModule } from '@angular/common/http';
import { UsuarioService } from '../../services/usuario/usuario.service';
import { RegionService } from '../../services/region/region.service';
import { Region } from '../../interfaces/region';
import { TOAST_TYPE } from '../../shared/components/toast/toast.component';
import { ToastService } from '../../services/toast/toast.service';

@Component({
  selector: 'app-home',
  standalone: true,
  providers: [UsuarioService, RegionService],
  imports: [CommonModule, HttpClientModule],
  templateUrl: './home.component.html',
  styleUrl: './home.component.css'
})
export class HomeComponent implements OnInit {
  isLoading: boolean = true;
  regiones: Region[] = [];

  constructor(private router: Router, private usuarioService: UsuarioService, private regionService: RegionService, private toastService: ToastService) { }

  logout() {
    sessionStorage.clear();
    this.router.navigate(['/login']);
  }

  goToReserva(selected_region: Region) {
    sessionStorage.setItem('selected_region', JSON.stringify(selected_region));
    this.router.navigate(['/reserva']);
  }

  goToAdmin() {
    this.router.navigate(['/admin']);
  }

  isAdmin() {
    const usuarioString = sessionStorage.getItem('usuario');
    if (usuarioString) {
      const usuario = JSON.parse(usuarioString);
      if (usuario.rol === 'administrador') {
        return true;
      }
    }
    return false;
  }

  loadRegiones() {
    this.regionService.getRegiones().subscribe({
      next: (data: any) => {
        console.log(data);
        for (const region of data) {
          this.regiones.push(region.nombre);
        }
      },
      error: (error: any) => {
        console.log(error);
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
            this.toastService.showToast('Su cuenta aún no ha sido aprobada', 'Contacte con el administrador para que apruebe su cuenta.', TOAST_TYPE.error);
          } else if (data.estado_invitado === 'invitado') {
            router.navigate(['update-profile']);
            this.toastService.showToast('Complete su perfil', 'Por favor, complete su perfil para poder realizar reservas.', TOAST_TYPE.info);
          }
          this.isLoading = false;
          this.loadRegiones();
        },
        error: (error: any) => {
          console.log(error);
          sessionStorage.clear();
          router.navigate(['login']);
        }
      });
    }
  }
}
