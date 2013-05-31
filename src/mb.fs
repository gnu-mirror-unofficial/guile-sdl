;;; mb.fs --- mouse buttons                           -*- scheme -*-

;; Copyright (C) 2013 Thien-Thi Nguyen
;;
;; This is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.
;;
;; This software is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this package; see the file COPYING.  If not, write to the
;; Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
;; Boston, MA  02110-1301  USA

mb "mouse-button"
#:infile "SDL/SDL_mouse.h"
#:symrgx (def ws ("SDL_BUTTON_[A-Z0-9]+") ws "[0-9]")
#:kxform (11 downcase ("^wheel" 0 "-"))
#:v2mask "SDL_BUTTON (~A)"

;;; mb.fs ends here
