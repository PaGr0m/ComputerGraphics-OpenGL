# 3D объект

### Условие
- загрузка и отрисовка 3D модели формата obj с текстурами
- отражения и преломления environment'а (cubemap/sphere map)
- навигация камеры воркуг объекта.
    - в качестве образца берем логику навигации отсюда https://threejs.org/examples/#webgl_geometry_teapot
    - камера всегда смотрит в центр сцены (условно, lookat=(0, 0, 0))
    - mouse drag перемещает камеру по сфере с центром в lookat
    - mouse zoom - приближает/отдаляет камеру (меняет радиус сферы)

## Запуск 
Собираем проект:
1) Windows `build.bat` 
2) Linux `build.sh`

Далее запускаем `build/Task-2-3Dobject`

## Примеры
- *Without Fresnel coefficient*
![](screenshots/nanosuit_2.gif)

- *Without Fresnel coefficient*
![](screenshots/nanosuit.gif)

- *Without Fresnel coefficient*
![](screenshots/aircraft.gif)
