#include "projectfiles.h"
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <roversight/roversight.h>

int main(int argc, char* argv[]) {
    using namespace roversight;

    // Construct a RoverSight solver.
    RoverSight roverSight;

    // Load camera calibration from file.
    std::string calib_path = projectfiles::get("demo/data/webcam_intrinsic_boof.yaml");
    Camera cam(calib_path);

    fmt::print("{}", cam);

    ImageRGB img;
    img.from_file(projectfiles::get("demo/data/webcam_test.png"));
    img = img.undistort(cam);
    img.to_file("webcam_test.jpg");

    /*
    for(const auto& batch : batches) {
        for(const auto& thing : batch) {
            // Add images and their approximate poses to the solver.
            Pose p = getPose(thing);
            ImageRGB img = getImage(thing);
            roverSight.addImage(img, cam, p);
            roverSight.addImage(img, cam, p);
        }
        // Solve
        roverSight.updateMesh();
    }
    */

    // Get the latest mesh.
    auto mesh = roverSight.getMesh();

    // Save the mesh result to file.
    // save_ply("output.ply", mesh);

    return 0;
}
