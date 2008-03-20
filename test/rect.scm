;;; rect.scm --- simple rectangle test

(define debug? (getenv "DEBUG"))
(and debug? (debug-enable 'debug 'backtrace))

(use-modules ((sdl sdl) #:renamer (symbol-prefix-proc 'SDL:)))

;; initialize the SDL video module
(SDL:init '(SDL_INIT_VIDEO))

(and debug?
     (for-each (lambda (x) (fso "video-info: ~S\n" x))
               (SDL:get-video-info)))

;; get a sample rect size from a list of available modes
(define test-rect
  (let ((modes (SDL:list-modes)))
    (and debug? (fso "hmm: (SDL:list-modes) => ~A\n" modes))
    (cond ((eq? modes #f)
           (error "no supported video modes"))
          ((eq? modes #t)
           ;; any mode, use arbitrary 800x600
           (SDL:make-rect 0 0 800 600))
          (else
           ;; a list - choose the first mode
           (car modes)))))
(and debug? (fso "test-rect => ~A\n" test-rect))

(set! *random-state* (seed->random-state (current-time)))

(define (rand-rect limit)
  (let* ((limit-w (SDL:rect:w limit))
         (limit-h (SDL:rect:h limit))
         (two-x (list (random limit-w) (random limit-w)))
         (two-y (list (random limit-h) (random limit-h)))
         (x0 (apply min two-x))
         (y0 (apply min two-y))
         (x1 (apply max two-x))
         (y1 (apply max two-y)))
    (SDL:make-rect (- x1 x0 -1) (- y1 y0 -1) x0 y0)))

;; set the video mode to the dimensions of our rect
(define screen (SDL:set-video-mode (SDL:rect:w test-rect)
                                   (SDL:rect:h test-rect)
                                   16 '(SDL_HWSURFACE)))

;; draw some rectangles filled with random colors
(do ((i 0 (1+ i)))
    ((= i 20))
  (let ((sample (rand-rect test-rect)))
    (SDL:fill-rect screen sample (random #xffffff))
    (SDL:update-rect screen sample))
  (SDL:delay 100))

;; quit SDL
(SDL:quit)

;;; rect.scm ends here
