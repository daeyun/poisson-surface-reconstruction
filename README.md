## PoissonRecon

CMake build of http://www.cs.jhu.edu/~misha/Code/PoissonRecon/ with MATLAB MEX-File bindings

## Build

```
cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Release
make -j4 -Cbuild
```

## Usage

```
[faces, vertices] = poissonRecon(points, normals, depth, fullDepth, scale, samplesPerNode, cgDepth);
```

### Example

```
[x, y, z] = sphere(15);
pts = [x(:), y(:), z(:)];
normals = normalize(-pts);

figure;
drawPoint3d(pts);
drawEdge3d([pts, pts+normalize(normals)*0.15]);
axis equal; grid on;

[faces, vertices] = poissonRecon(pts, normals, 3);
figure;
trimesh(faces, vertices(:,1), vertices(:,2), vertices(:,3));
axis equal; grid on;
```

<img width="45%" src="doc/sphere_point_cloud.png" /><img width="45%" src="doc/sphere_mesh.png" />

## License

- PoissonRecon code: `(c) 2006 Michael Kazhdan and Matthew Bolitho`

    See license.txt

- mexutil.h: MPL 2.0. If this cmake/MATLAB build doesn't work (especially on windows), daeyun@daeyunshin.com
