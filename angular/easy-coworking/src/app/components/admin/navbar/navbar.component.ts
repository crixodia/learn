import { Component, OnInit, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { RouterModule } from '@angular/router';
import { HttpClientModule } from '@angular/common/http';
import { UsuarioService } from '../../../services/usuario/usuario.service';
import { Subscription, interval } from 'rxjs';

@Component({
  selector: 'app-navbar',
  standalone: true,
  providers: [UsuarioService],
  imports: [CommonModule, RouterModule, HttpClientModule],
  templateUrl: './navbar.component.html',
  styleUrl: './navbar.component.css'
})
export class NavbarComponent implements OnInit, OnDestroy {
  autoregistrados: number = 0
  private subscription: Subscription = new Subscription();

  constructor(private usuarioService: UsuarioService) { }

  countAutoregistrados(): void {
    this.autoregistrados = parseInt(sessionStorage.getItem('autoregistrados') || '0');
  }

  ngOnInit(): void {
    this.countAutoregistrados();

    const source$ = interval(5000);
    this.subscription = source$.subscribe(() => {
      this.countAutoregistrados();
    });
  }

  ngOnDestroy(): void {
    if (this.subscription) {
      this.subscription.unsubscribe();
    }
  }

  logout(): void {
    sessionStorage.clear();
    window.location.href = '/login';
  }

  goToHome(): void {
    window.location.href = '/home';
  }

}
